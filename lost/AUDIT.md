# AUDIT - minishell -> 42_sh

> Statut : projet en cours de conversion de `minishell` vers `42_sh`. Cet audit
> recense les points restant à traiter. Il ne conserve pas les éléments déjà
> corrigés.

Sommaire :
1. Fonctions maison conservées (et pourquoi).
2. Architecture : défauts restants.
3. Testabilité et automatisation.
4. Tokenisation.
5. Factorisation (helpers) : duplications à extraire.
6. Extensibilité : ajouter un builtin/opérateur/redirection facilement.

---

## Partie 1 - Fonctions maison conservées

Ces réimplémentations restent en place, non par contrainte de Norme (levée en
42_sh) mais parce qu'elles n'ont pas d'équivalent libc direct, ou que le
remplacement serait incorrect.

| Fonction | Raison de conservation |
|---|---|
| `ft_split` ([ft_split.c](src/utils/std/ft_split.c)), `ft_strjoin` ([ft_strjoin.c](src/utils/std/ft_strjoin.c)) | Aucun équivalent libc en un appel. `asprintf` (autorisé en 42_sh) modifierait l'API et la sémantique d'erreur. |
| `ft_itoa` ([ft_toa.c](src/utils/std/ft_toa.c)), `ft_atoll_secure` ([ft_atoll.c](src/utils/std/ft_atoll.c)) | Minces wrappers libc (`snprintf`+`strdup` ; `strtoll`+`errno`). Pas d'`itoa` en C standard ; `strtoll` enveloppé pour le drapeau d'erreur. |
| `free_char_array` ([ft_free.c](src/utils/std/ft_free.c)) | Pas d'équivalent libc pour libérer un `char **` terminé par `NULL`. |
| `nano_get_next_line` ([nano_gnl.c](src/utils/gnl/nano_gnl.c)) | `getline` bufferise un `FILE*` et sur-lirait `STDIN_FILENO` (vol de l'entrée des enfants / du heredoc). Lecture octet par octet obligatoire sur un fd partagé. |
| `ft_wput*` ([src/utils/wput/](src/utils/wput/)) | Primitives d'écho UTF-8 avec gestion `EPIPE` ; pas d'équivalent libc en un appel. |

Note : `ft_std.h` n'est plus qu'un pont vers `<string.h>`/`<ctype.h>`/... plus
quelques helpers ; il pourra être dissous.

---

## Partie 2 - Architecture : défauts restants

### 2.1 - `<stdio.h>` inclus dans ~16 fichiers d'exécution sans usage

Inclus dans [exec_cmd.c:24](src/exec/exec_cmd.c#L24),
[exec_pipe.c:17](src/exec/exec_pipe.c#L17),
[bt_cd.c:17](src/exec/builtin/bt_cd.c#L17),
[bt_pwd.c:18](src/exec/builtin/bt_pwd.c#L18),
[redirs_apply.c:22](src/exec/redirs_apply.c#L22)... alors que seul
`ft_printf_fd` maison est utilisé. Includes morts (reliquats de debug `printf`).
[sort.c:16-17](src/expand/wildcard/sort.c#L16-L17) inclut `<string.h>`
et `<stdio.h>` tout en appelant des `ft_*` maison : incohérent. À nettoyer.

### 2.2 - Header qui s'inclut lui-même

[module_minishell.h:16](include/module_minishell.h#L16) : `# include "module_minishell.h"`.
Sans effet (header guard) mais symptôme d'un graphe d'inclusion non maîtrisé :
`module_minishell.h` tire 6 autres modules, et presque chaque `.c` ré-inclut une
dizaine de headers.

### 2.3 - Header guard détourné pour protéger un include système

[module_minishell.h:23-26](include/module_minishell.h#L23-L26) :

```c
# ifndef SIGNAL_HANDLER_H
#  define SIGNAL_HANDLER_H
#  include <signal.h>
# endif
```

`<signal.h>` a son propre guard interne : ce wrapper est inutile et fragile (si
un autre fichier définit `SIGNAL_HANDLER_H`, `signal.h` ne s'inclut plus).
`# include <signal.h>` suffit.

### 2.4 - Paramètre `lv` : nombres magiques propagés

`lv = 7` codé en dur ([launcher.c:66](src/core/launcher.c#L66)) puis passé dans toute
la chaîne : `minishell(mns, lv)`, `run_parser(..., lv)`, `checker_init(..., lv)`,
`lexer_init(..., lv)`... On trouve aussi `parser_free(&mns->parser, 255)`
([minishell_lifecycle.c:46](src/core/shell/minishell_lifecycle.c#L46)). Les
valeurs (7, 255) n'ont ni constante nommée ni commentaire.

**Attendu :** une `enum`/`#define` documentée, ou un rangement dans `t_mns`
(`mns->level` existe mais est initialisé à `0`,
[minishell_lifecycle.c:76](src/core/shell/minishell_lifecycle.c#L76)) plutôt qu'un
paramètre traîné partout.

### 2.5 - `write()` avec longueur codée en dur (bug réel)

[launcher.c:62](src/core/launcher.c#L62) : `write(STDERR_FILENO, "Usage: ./minishell\n", 20);`
La chaîne fait 19 octets, pas 20 : un octet de trop (lecture hors limites du
littéral). Les autres comptes sont justes mais le principe est fragile.

**Attendu :** `write(fd, s, ft_strlen(s))`, jamais une constante recomptée.

### 2.6 - Échecs d'allocation avalés silencieusement

`sstrs_append` ([sstr.c:88-90](src/utils/sstr/sstr.c#L88-L90)) fait un `return;`
muet si le `malloc` échoue : perte de données silencieuse. `nano_get_next_line`
retourne `NULL` indistinctement pour EOF, erreur de `read` et échec de `malloc` :
l'appelant ne peut pas différencier fin normale et échec.

### 2.7 - Ordre du pipeline surprenant

Le checker (`check/`) s'exécute avant le lexer (`lex/`). Valider la syntaxe avant
de tokeniser est inhabituel ; d'ordinaire on lexe, puis on parse, et la validation
tombe du parsing. À documenter ou à réordonner.

---

## Partie 3 - Testabilité et automatisation

Objectif : figer le comportement actuel avant transformation, et détecter une
régression immédiatement.

### 3.1 - État partagé : global assumé, pas propagation

Décision retenue : un shell est mono-instance ; l'état global (`g_shell`) est
assumé, comme dans bash. La propagation complète de `t_mns *` n'est justifiée que
pour une bibliothèque réentrante, ce que 42_sh n'est pas.

Conséquence pour les tests : une globale ne bloque pas les tests unitaires.
Criterion fork chaque test dans son process, donc pas de contamination ; le
fixture arme `g_shell` avant l'appel. La dépendance reste implicite (non visible
dans la signature) : c'est le seul inconvénient, mineur.

### 3.2 - Couches pures isolables

Chaque étape du pipeline devrait exposer une fonction sans effet de bord :
entrée -> structure de sortie, sans I/O ni `fork`. C'est presque le cas pour
[lex/](src/lex/), [parse/](src/parse/), [expand/](src/expand/). Il faut garantir qu'on peut linker un
seul module dans un binaire de test, donc casser le « tout inclut tout » (cf.
2.2).

Cible :
```c
t_token_lst *lex(const char *line, const char *ifs);
t_ast_node  *parse(t_token_lst *toks);
char        *expand(const char *raw, t_ctx *ctx);
```

### 3.3 - Séparer décision et effet de bord

L'exécution est peu testable car la décision (quoi lancer, quels fd, quelles
redirs) et l'action (`fork`/`execve`/`dup2`) sont mêlées
([exec_cmd_external.c:18-41](src/exec/exec_cmd_external.c#L18-L41)).

**Attendu :** une fonction pure construit le plan (chemin résolu, table de
redirections, argv final) ; une autre l'exécute. La première se teste par diff.
Pour la lecture, prévoir une frontière où injecter une chaîne au lieu d'un
terminal, et lever `interactive_shell_only`
([launcher.c:44-52](src/core/launcher.c#L44-L52)) en mode test.

### 3.4 - Pile de test

```
tests/
  unit/            # couches pures (Criterion)
  characterization/# fige le comportement actuel avant transformation
  e2e/             # testeur différentiel existant
```

- Criterion : chaque test dans son propre fork (un segfault n'emporte pas la
  suite). Sortie TAP.
- Tests de caractérisation : capturer ce que le lexer/parser produit aujourd'hui,
  filet pendant le refactor.
- E2E : le testeur différentiel devient une cible `make test-e2e`.

### 3.5 - Automatisation

| Mécanisme | Effet |
|---|---|
| `make test` (< 1 s, unitaires) | feedback immédiat |
| hook git `pre-commit` | bloque le commit si rouge |
| file watcher (`entr`) | relance à chaque sauvegarde |
| CI sur `push` | `make test` + e2e + valgrind |
| `make valgrind` (`--trace-children=yes`) | fuites après `fork` |

Prévoir la normalisation des sorties non déterministes (`$$`, `$!`, chemins
absolus, prompt) pour les e2e.

---

## Partie 4 - Tokenisation

Le modèle est correct, l'implémentation autour ne l'est pas. Le découpage d'un
mot en *parts* (segments porteurs de leur contexte de quoting) est la bonne
fondation pour 42_sh. Les problèmes sont dans la plomberie.

### À conserver

- **Modèle « parts »** : un mot `ab"cd"$X'ef'` = un token composé de plusieurs
  `t_tk_part`, chacune avec son type de quote et ses flags
  ([module_token.h:76-88](include/module_token.h#L76-L88)). Contexte de quoting
  conservé pour différer l'expansion.
- **Abstraction `t_input`** (`peek`/`advance`/`match_str`,
  [module_input.h](include/module_input.h)).
- **États pendants multi-lignes** (`pending_dquote`/`pending_squote`/`pending_escape`,
  [module_lexer.h:34-36](include/module_lexer.h#L34-L36)).

### 4.1 - L'entrée est détruite pendant le lexing

`erase_and_advance` écrit des `\0` dans le stream
([inp_adv.c:47-55](src/lex/input/inp_adv.c#L47-L55)), idem `erase_and_advance_str`
([inp_adv.c:32-45](src/lex/input/inp_adv.c#L32-L45)). Les parts stockent un
`offset` dans un buffer mutilé au fur et à mesure, et il faut repasser la `line`
d'origine séparément.

**Attendu :** entrée en lecture seule, copier les sous-chaînes au lieu de
poinçonner le buffer.

### 4.2 - Classifications redondantes

`kind` et `family` : **résolu**. Champs supprimés de `t_token`, enums
`t_tk_kind`/`t_tk_family` supprimés. Remplacés par des **prédicats** dérivés du
type ([classify.c](src/grammar/classify.c)) : `is_word`, `is_redirect`,
`is_control`, `is_operator`, `is_operand`, `is_subshell`, `is_connector`. Une
seule source de vérité, le `type`. (Nuance : bash dérive ça de sa grammaire yacc
et de `switch` sur enums, pas de prédicats `is_*` ; ici les prédicats sont un
idiome C lisible qui suit le même principe.) `group` **reste** un champ : état
contextuel réel (position du token), posé à l'init, réaffecté par le parser.

Reste : les flags `has_dollar`/`has_tilde`/`has_glob` dupliqués entre `t_token`
et chaque `t_tk_part`.

### 4.3 - Le lexer fait de l'I/O et dépend de `lv`

- `isatty(STDIN_FILENO)` décide d'émettre `TOK_NEWLINE`
  ([lx_run.c:61](src/lex/lx_run.c#L61)) : le flux de tokens dépend du
  terminal.
- `if (lv < 3) return;` ([lx_run.c:47](src/lex/lx_run.c#L47)) : `lv`
  (cf. 2.4) remonte jusque dans le lexer.

**Attendu :** sortir `isatty` et `lv` du tokenizer ; la décision d'émettre un
newline appartient à l'appelant.

### 4.4 - Bugs ponctuels

- **Variable non initialisée** : `bool had_char;`
  ([lex_unquoted.c:59](src/lex/lex_unquoted.c#L59)) est lue
  ([lex_unquoted.c:65](src/lex/lex_unquoted.c#L65)) sans init si le
  premier caractère est un opérateur.
- **`get_ptr` retourne `false`** (un `bool`) là où le type est `char *`
  ([inp_adv.c:19](src/lex/input/inp_adv.c#L19)) : devrait être `NULL`.
- **Skip de commentaire** : `#` s'arrête sur un backslash
  ([lex_unquoted.c:49](src/lex/lex_unquoted.c#L49)) au lieu de courir
  jusqu'au `\n`.

### 4.5 - Priorités

1. 4.3 (sortir I/O et `lv`) : débloque le TDD.
2. 4.1 (entrée en lecture seule).
3. 4.2 puis 4.4.

---

## Partie 5 - Factorisation (helpers)

Duplications récurrentes extractibles en helpers. Les gains sont surtout autour
de la création de tokens et de l'allocation.

### 5.1 - Allocation-ou-échec (`xmalloc` / `xcalloc`)

Le motif « alloue, et abandonne si `NULL` » apparaît **27 fois** :

```c
p = malloc(n);
if (!p)
    free_and_exit_minishell(EXIT_FAILURE);
```

(env, `m_token`, `m_ast`, ...). Un helper qui abandonne en interne réduit chaque
site à `p = xmalloc(n);` / `p = xcalloc(n, sz);`. Gain : ~27 blocs de 2-3 lignes,
et un seul point de gestion d'échec.

### 5.2 - `token_new` : contrat « jamais NULL » + classification dérivée

`token_new` ([token_lifecycle.c:18](src/token/token_lifecycle.c#L18)) abandonne
déjà en interne sur échec de `calloc`. Pourtant chaque appelant refait le test :

```c
token = token_new(type);
if (!token)                       /* mort : token_new sort avant */
    free_and_exit_minishell(...);
token->kind   = TKD_...;          /* posé à la main partout */
token->family = TKF_...;
token->group  = TKG_...;
```

Classification : **faite**. `token_new` pose `kind`/`family`/`group` via
[classify.c](src/grammar/classify.c) ; les ~18 triplets manuels ont été retirés
(et corrigent au passage les tokens de split, qui n'étaient pas classifiés).

Reste : les **8 blocs `if (!token)` morts** chez les appelants (`token_new` ne
renvoie jamais `NULL`, il abandonne avant), à supprimer.

### 5.3 - Recherche de queue de liste

Le motif `while (cur->next) cur = cur->next;` (aller à la fin d'une liste
chaînée) apparaît **8 fois**. Un `*_tail()` par type de liste (`t_token`,
`t_tk_part`, env) le factorise. Marginal mais récurrent.

### 5.4 - Assemblage de chaîne depuis des parts

`aggregate_wordparts_to_strline` ([token_content.c](src/token/token_content.c))
fait un `malloc` + boucle `memcpy` manuelle en deux passes (mesure puis copie).
Le buffer `exstr` (croissance amortie) le ferait en une passe sans pré-calcul de
longueur.

---

## Partie 6 - Extensibilité (architecture team-friendly)

Objectif : ajouter un builtin, un opérateur ou une redirection doit être
**localisé** (un endroit évident) et **déclaratif** (une ligne de table plus une
fonction), et non une modification de cascades éparpillées. C'est l'équivalent C
d'une interface + registre en Java.

État : **en place**. `src/` réorganisé en une convention unique
(`read/lex/token/check/parse/ast/expand/assemble/exec/`, plus `core/`, `env/`,
`utils/`, `grammar/`). Makefile en auto-découverte (`find`). Les tables sont
écrites : [operators.c](src/grammar/operators.c) (reconnaissance),
[tok_to_ast.c](src/grammar/tok_to_ast.c) (TOK -> AST), et le dispatch
`AST -> handler` dans [exec_run.c](src/exec/exec_run.c). `exec/` est scindé en
`exec/builtin/`, `exec/control/`, `exec/redir/`. Les cascades de `if` ont été
supprimées ; ajouter un opérateur/redirection est désormais déclaratif.

### Modèle de référence : les builtins (déjà correct)

[execute_builtin.c:20-33](src/exec/execute_builtin.c#L20-L33) :

```c
static t_builtin_entry	builtins[] = {
    {"cd", builtin_cd}, {"pwd", builtin_pwd}, {"echo", builtin_echo}, ...
    {NULL, NULL}
};
```

Ajouter un builtin = une ligne dans la table + une fonction. `is_builtin` et
`execute_builtin` parcourent la table génériquement. C'est le patron à répliquer.

### Opérateurs et redirections : aujourd'hui en cascades

Un opérateur traverse quatre couches, chacune en cascade de `if` :

| Couche | Emplacement |
|---|---|
| Reconnaissance (lexer) | `lex_operator` / `lex_redirection` ([lx_tokenize_op_rd.c:39-90](src/lex/lx_tokenize_op_rd.c#L39-L90)) |
| Classification | `kind`/`family`/`group` posés à la main (cf. 4.2, 5.2) |
| `TOK_*` -> `AST_*` | [convert.c:17-53](src/parse/convert.c#L17-L53) (trois cascades) |
| Exécution | `exec_node` ([exec_run.c:39-43](src/exec/exec_run.c#L39-L43)) |

Une redirection traverse : reconnaissance (même cascade), classification, puis
application ([redirs_apply.c:76-80](src/exec/redirs_apply.c#L76-L80),
`if type == TOK_REDIR_OUT / IN / APPEND`).

### Cible : une table par point de dispatch

1. **Reconnaissance** (lexer) — une table triée par longueur (résout aussi 4.3) :

   ```c
   static const t_op_spec  OPS[] = {
       {"&&", TOK_AND_IF,        OPC_CONTROL},
       {"||", TOK_OR_IF,         OPC_CONTROL},
       {"|",  TOK_PIPE,          OPC_CONTROL},
       {";",  TOK_SEMI,          OPC_CONTROL},
       {">>", TOK_REDIR_APPEND,  OPC_REDIR},
       {"<<", TOK_REDIR_HEREDOC, OPC_REDIR},
       {">",  TOK_REDIR_OUT,     OPC_REDIR},
       {"<",  TOK_REDIR_IN,      OPC_REDIR},
       {NULL, 0, 0}
   };
   ```
   Le lexer scanne la table (plus long d'abord) au lieu des cascades.

2. **Classification** — une table `type -> {kind, family, group}` interrogée par
   `token_new(type)` (cf. 5.2). Plus aucune pose manuelle.

3. **`TOK_*` -> `AST_*`** — une table `{TOK_*, AST_*}` remplace les trois cascades
   de `convert.c`.

4. **Exécution** — une table `{AST_*, handler}` comme les builtins :

   ```c
   static const t_ast_exec  EXECS[] = {
       {AST_PIPE, exec_pipeline}, {AST_AND, exec_and_or},
       {AST_OR, exec_and_or},     {AST_SEQ, exec_sequence},
       {AST_SUBSHELL, exec_subshell}, {0, NULL}
   };
   ```

5. **Application des redirections** — une table `{TOK_REDIR_*, open_flags, apply}`
   remplace la cascade de `redirs_apply`.

### Coût d'ajout après refonte

| Ajouter... | Avant | Après |
|---|---|---|
| builtin | 1 ligne + fonction | inchangé (déjà bon) |
| opérateur | ~6 cascades | 1 ligne dans `OPS` + 1 ligne `TOK->AST` + 1 ligne `EXECS` (+ handler) |
| redirection | ~4 cascades | 1 ligne dans `OPS` + 1 ligne dans la table d'application |

Les tables sont côte à côte, donc « où ajouter » est immédiat, et un ajout ne
peut pas oublier une couche (chaque table incomplète se voit).

### Reste à faire

Toutes les tables sont en place : `OPS` ([operators.c](src/grammar/operators.c)),
classification ([classify.c](src/grammar/classify.c), posée par `token_new`),
`TOK->AST` ([tok_to_ast.c](src/grammar/tok_to_ast.c)), `AST->handler`
([exec_run.c](src/exec/exec_run.c)), application des redirections
([redirs_apply.c](src/exec/redir/redirs_apply.c)).

Ajouter un opérateur/redirection 42_sh (`&`, `;;`, `<>`, `>|`, `&>`, `2>`...) se
fait maintenant en ajoutant une ligne dans les tables concernées (+ un handler
pour un nouvel opérateur de contrôle).

---

## Ce qui est correct (à conserver)

- Découpage du flux en étapes (reading -> ... -> execution).
- Une seule vraie globale de handler `g_signal_flag`
  ([init.c:19](src/core/signal/init.c#L19)), `volatile sig_atomic_t`.
- Signaux fork/parent : `sig_parent_ignore`/`sig_parent_restore` autour du
  `waitpid` ([exec_cmd_external.c:37-39](src/exec/exec_cmd_external.c#L37-L39)),
  `sig_reset` dans l'enfant.
- Structures réutilisables (deque [ft_deque.h](include/ft_deque.h), env en liste
  chaînée).

---

## TL;DR

| Sujet | Action |
|---|---|
| `<stdio.h>` morts, auto-include, guard sur `signal.h` | Nettoyer le graphe d'includes (2.1-2.3) |
| `lv` magique, `255`, `write(..., 20)` | Constantes nommées, `ft_strlen` (2.4-2.5) |
| Échecs `malloc` muets | Propager les erreurs (2.6) |
| Factorisation | `xmalloc`/`xcalloc` ; `token_new` qui pose la classification ; `*_tail()` (Partie 5) |
| Extensibilité | tables de dispatch (reconnaissance, `TOK->AST`, `AST->handler`, redirs) sur le modèle des builtins (Partie 6) |
| Tokenisation | Entrée lecture seule, sortir `isatty`/`lv`, dédup des classifications (Partie 4) |
| Tests | Couches pures isolables, Criterion, `make test`, hook `pre-commit` (Partie 3) |
