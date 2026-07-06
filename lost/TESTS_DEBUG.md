# TESTS & DEBUG - guide pratique

> Compagnon de [AUDIT.md](AUDIT.md). L'audit dit *quoi* découpler pour pouvoir tester ; ce fichier dit *avec quels outils* on teste et on débogue concrètement, et répond à deux questions : " peut-on avoir un mode dev/prod en C ? " et " peut-on tester des étapes qui produisent des fichiers (tokens, AST...) ? ". Réponse aux deux : **oui, et c'est la pratique standard.**

---

## Partie A - Le mode dev/prod en C : pas du bricolage

C **a prévu ça depuis le début**. Le mécanisme n'est pas un framework comme en JS/Python, mais des briques de première classe. Mieux : le standard C définit lui-même une macro de prod, `NDEBUG`.

Il y a **trois niveaux** de " mode ", à combiner :

### A.1 - Compilation conditionnelle (préprocesseur) - le cœur

```c
#ifdef MNSH_DEBUG
    dump_tokens(stderr, &lx->tokens);   /* compilé seulement en dev */
#endif
```

Le code entre `#ifdef MNSH_DEBUG ... #endif` **n'existe pas** dans le binaire de prod : zéro coût, zéro octet. On active la macro à la compilation : `cc -DMNSH_DEBUG ...`. C'est le mécanisme idiomatique, là depuis K&R.

Un helper pratique pour ne pas semer des `#ifdef` partout :

```c
/* include/debug.h */
#ifdef MNSH_DEBUG
# define DBG(...)      ft_printf_fd(2, __VA_ARGS__)
# define DBG_DUMP(fn)  do { fn; } while (0)
#else
# define DBG(...)      ((void)0)
# define DBG_DUMP(fn)  ((void)0)
#endif
```

-> en prod, `DBG("pos=%d\n", pos)` se volatilise ; en dev il écrit sur stderr.

### A.2 - `NDEBUG` et `assert` (standard C)

`<assert.h>` est le mode dev/prod **dans le standard** : si `NDEBUG` est défini, `assert(x)` devient un no-op ; sinon il vérifie `x` et abort avec fichier/ligne si faux.

```c
assert(token->type != TOK_EOF);   /* dev : vérifie ; prod (-DNDEBUG) : disparaît */
```

> ⚠️ En 42 " pur " `assert` était hors-liste ; en **42sh toute la libc est autorisée** -> tu peux t'en servir librement (cf. AUDIT Partie 1).

### A.3 - Profils de build (Makefile) - le commutateur réel

On ne tape jamais les flags à la main : on fait des cibles. Voir la Partie F pour le Makefile complet. Idée :

| Cible | Flags | Usage |
|---|---|---|
| `make` / `make release` | `-O2 -DNDEBUG` | prod : rapide, asserts off, pas de debug |
| `make debug` | `-g3 -O0 -DMNSH_DEBUG` | dev : symboles gdb, dumps actifs |
| `make asan` | `-g -O1 -fsanitize=address,undefined` | chasse mémoire/UB |
| `make test` | build + lance les tests unitaires | TDD |

### A.4 - Bascule au runtime (variable d'environnement)

Pour activer le debug **sans recompiler** (utile quand le bug n'apparaît qu'en prod) :

```c
static bool debug_on(void)
{
    static int v = -1;
    if (v == -1)
        v = (getenv("MNSH_DEBUG") != NULL);
    return (v);
}
...
if (debug_on())
    dump_ast(stderr, node);
```

-> `MNSH_DEBUG=1 ./minishell`. Coût négligeable, et ça reste dans le binaire de prod (à toi de voir si tu veux ça).

**Règle :** compile-time (`#ifdef`) pour ce qui doit **disparaître** en prod (dumps verbeux, asserts) ; runtime (`getenv`) pour ce que tu veux pouvoir **rallumer à chaud**.

---

## Partie B - Dumper les représentations intermédiaires (ta vraie demande)

Tu veux inspecter tokeniseur -> lexer -> parser -> AST. **C'est exactement ce que font les compilateurs** : `gcc -fdump-tree-all`, `clang -emit-ast`, `-Xclang -ast-dump`. Le principe : des fonctions de **sérialisation** des structures intermédiaires vers un flux (fichier ou stderr), activées en mode dev.

### B.1 - Dump des tokens (texte, 1 par ligne)

```c
/* src/debug/dump_tokens.c */
void dump_tokens(int fd, t_tokens *toks)
{
    t_token *t = toks->head;
    while (t)
    {
        ft_printf_fd(fd, "[%-14s] '%s'%s%s%s\n",
            tok_type_name(t->type), t->str ? t->str : "",
            t->has_dollar ? " $" : "",
            t->has_tilde  ? " ~" : "",
            t->has_glob   ? " *" : "");
        t = t->next;
    }
}
```

Tu as besoin d'un `tok_type_name(t_tk_type)` qui mappe l'enum -> string. **Bonus :** cette fonction est aussi ce qui permet de simplifier les 4 classifications (cf. AUDIT 4.2).

### B.2 - Dump de l'AST (arbre indenté)

```c
/* src/debug/dump_ast.c */
void dump_ast(int fd, t_ast_node *n, int depth)
{
    if (!n)
        return ;
    for (int i = 0; i < depth; i++)
        ft_printf_fd(fd, "  ");
    ft_printf_fd(fd, "%s\n", ast_kind_name(n->kind));
    dump_ast(fd, n->left,  depth + 1);
    dump_ast(fd, n->right, depth + 1);
}
```

### B.3 - Dump de l'AST en Graphviz (visuel) - le plus utile

Génère un `.dot`, rends-le en image. Voir un AND/OR/PIPE arborescent visuellement vaut 1000 lignes de log :

```c
/* émet du DOT ; puis : dot -Tpng ast.dot -o ast.png */
void dump_ast_dot(int fd, t_ast_node *n)
{
    ft_printf_fd(fd, "digraph AST {\n");
    dump_ast_dot_rec(fd, n);
    ft_printf_fd(fd, "}\n");
}
```

```sh
./minishell_debug -c 'a && b | c' --dump-ast > ast.dot && dot -Tpng ast.dot -o ast.png
```

### B.4 - Où brancher les dumps

Dans [minishell.c:24-40](src/minishell/minishell.c#L24-L40), entre les étapes :

```c
run_lexer(...);          DBG_DUMP(dump_tokens(2, &mns->lexer.tokens));
run_parser(...);         DBG_DUMP(dump_ast(2, node, 0));
run_exec(node, lv);
```

-> en prod ces lignes n'existent pas.

---

## Partie C - Tester ce qui " produit des fichiers " : snapshot / golden tests

Tu ne hallucines pas : tester une étape via le fichier qu'elle produit est une technique nommée - **snapshot testing** (ou *golden file* / *characterization test*, déjà évoqué AUDIT 3.4). C'est précisément comme ça que les suites de tests des compilateurs marchent.

### Principe

1. Tu lances ton étape sur une entrée -> elle **dump** sa sortie (tokens / AST).
2. La **première fois**, tu vérifies à la main que c'est correct et tu **commits** ce dump comme " référence " (golden).
3. À chaque run de test : re-dump -> `diff` avec le golden. **Différent = régression = test rouge.**

```
tests/snapshots/
  lexer/
    pipe_and_quotes.sh        # l'entrée :  echo "a b" | cat
    pipe_and_quotes.expected  # le dump de référence (golden), commité
  ast/
    and_or_precedence.sh
    and_or_precedence.expected
```

### Le runner (idée)

```sh
for case in tests/snapshots/lexer/*.sh; do
    got=$(./minishell_debug --dump-tokens < "$case")
    exp=$(cat "${case%.sh}.expected")
    if [ "$got" != "$exp" ]; then
        echo "FAIL $case"; diff <(echo "$exp") <(echo "$got")
    fi
done
```

### Mettre à jour les goldens (quand le changement est *voulu*)

```sh
UPDATE_SNAPSHOTS=1 make test-snapshots   # régénère les .expected
git diff tests/snapshots/                 # tu RELIS le diff avant de committer
```

> Le piège : ne jamais régénérer un golden sans relire le diff, sinon tu " valides " une régression. Le `git diff` est ta relecture.

### Pourquoi c'est parfait pour toi

Tes étapes lexer/parser/AST sont **déterministes** (entrée -> sortie, pas de hasard, pas de PID). Le snapshot testing y est idéal - **à condition** d'avoir réglé AUDIT 4.4 (sortir `isatty`/`lv` du lexer) et 2.1 (`get_mns`), sinon le dump dépend du terminal et n'est pas reproductible.

---

## Partie D - La boîte à outils (qui sert à quoi)

| Outil | Rôle | Commande type |
|---|---|---|
| **flags `cc`** | mode dev/prod | `-g3 -O0 -DMNSH_DEBUG` vs `-O2 -DNDEBUG` |
| **AddressSanitizer** | use-after-free, overflow, leaks, **traverse fork** | `-fsanitize=address` |
| **UBSan** | comportements indéfinis (ton `had_char` non init, AUDIT 4.5) | `-fsanitize=undefined` |
| **valgrind / memcheck** | fuites, surtout après `fork` | `valgrind --leak-check=full --trace-children=yes` |
| **gdb** | pas-à-pas, backtrace sur crash | `gdb --args ./minishell` ; `set follow-fork-mode child` |
| **Criterion** | framework de tests unitaires C (chaque test forké) | `cc ... -lcriterion` |
| **Graphviz `dot`** | visualiser l'AST | `dot -Tpng ast.dot -o ast.png` |
| **`diff` / `git diff`** | comparer dump vs golden | `diff exp got` |
| **`entr`** | relancer les tests à chaque save | `ls src/**/*.c \| entr -c make test` |
| **strace / ltrace** | tracer syscalls (debug exec/pipe/dup2) | `strace -f -e trace=execve,pipe,dup2 ./minishell` |

> ASan **et** valgrind font un travail proche ; ASan est plus rapide et meilleur sur la pile, valgrind ne demande aucune recompilation et reste la référence pour les fuites après `fork`. Avoir les deux cibles est sain.

---

## Partie E - Organisation des fichiers proposée

```
include/debug.h               # macros DBG / DBG_DUMP, prototypes des dumps
src/debug/                    # compilé SEULEMENT en mode debug (cf. Makefile)
  dump_tokens.c
  dump_ast.c
  dump_ast_dot.c
  type_names.c                # tok_type_name(), ast_kind_name()
tests/
  unit/                       # Criterion : couches pures (lexer, parser, expand)
  snapshots/                  # golden tests (Partie C)
    lexer/  ast/  expand/
  e2e/                        # ton testeur externe branché ici
```

Le dossier `src/debug/` n'est **ajouté aux sources que pour les cibles dev** (`make debug`, `make test`) -> en prod il n'est même pas compilé.

---

## Partie F - Recettes Makefile

À adapter à ton Makefile existant (qui gère déjà `DLVL`). Idée des cibles :

```make
# --- profils ---
release:  CFLAGS += -O2 -DNDEBUG
release:  $(NAME)

debug:    CFLAGS += -g3 -O0 -DMNSH_DEBUG
debug:    SRCS   += $(DEBUG_SRCS)          # ajoute src/debug/*.c
debug:    $(NAME)

asan:     CFLAGS += -g -O1 -fsanitize=address,undefined
asan:     LDFLAGS += -fsanitize=address,undefined
asan:     $(NAME)

# --- tests ---
test: debug
	$(MAKE) -C tests unit
	$(MAKE) -C tests snapshots

test-snapshots: debug
	./tests/run_snapshots.sh        # respecte UPDATE_SNAPSHOTS=1

valgrind: debug
	valgrind --leak-check=full --trace-children=yes ./$(NAME)

watch:
	ls src/**/*.c include/*.h | entr -c $(MAKE) test

# --- visualisation AST ---
ast-dump: debug
	echo "$(CMD)" | ./$(NAME) --dump-ast > ast.dot && dot -Tpng ast.dot -o ast.png
```

```sh
make debug                       # binaire avec dumps + symboles
make asan                        # chasse mémoire/UB
make test                        # unitaires + snapshots
make ast-dump CMD='a && b | c'   # génère ast.png
make watch                       # TDD en continu
```

---

## TL;DR

- **Mode dev/prod en C = standard, pas bricolage** : `#ifdef MNSH_DEBUG` (compile-time), `NDEBUG`/`assert` (le standard lui-même), profils Makefile, + `getenv` pour la bascule à chaud.
- **Dumper tokens/AST = pratique des vrais compilos** : fonctions de sérialisation guardées par `DBG_DUMP`, format texte ou Graphviz `.dot`.
- **Tester via fichiers produits = snapshot/golden testing** : dump -> `diff` avec une référence commitée ; `UPDATE_SNAPSHOTS=1` pour régénérer (en relisant le `git diff`).
- **Prérequis** (cf. AUDIT) : régler 2.1 (`get_mns`) et 4.4 (`isatty`/`lv`) pour que les dumps soient **déterministes**, sinon les snapshots sont instables.
