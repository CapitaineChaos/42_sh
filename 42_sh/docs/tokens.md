Voici un inventaire plus exhaustif des tokens que ton lexer peut émettre, organisé par grandes catégories ; les **WORD_PART** restent des tokens atomiques, tandis que **WORD** n’apparaît qu’au parseur :

---

### 1. Mots‑clés (recognized as distinct tokens by the parser)

| Token       | Lexème(s)                             | Rôle                                        |
|-------------|---------------------------------------|---------------------------------------------|
| `IF`        | `if`                                  | début d’un test conditionnel                |
| `THEN`      | `then`                                | sépare condition et bloc                    |
| `ELSE`      | `else`                                | branche alternative                        |
| `ELIF`      | `elif`                                | condition supplémentaire                   |
| `FI`        | `fi`                                  | fin de test                                 |
| `CASE`      | `case`                                | début d’un switch                           |
| `ESAC`      | `esac`                                | fin d’un switch                             |
| `FOR`       | `for`                                 | boucle for                                  |
| `IN`        | `in`                                  | sép. variable et liste                      |
| `DO`        | `do`                                  | début de bloc de boucle                     |
| `DONE`      | `done`                                | fin de bloc de boucle                       |
| `WHILE`     | `while`                               | début boucle conditionnelle                 |
| `UNTIL`     | `until`                               | début boucle inverse                        |
| `FUNCTION`* | `function`                            | déclaration de fonction (bash)              |
| `SELECT`*   | `select`                              | itération avec menu (bash interactif)       |
| `TIME`*     | `time`                                | builtin de mesure temporelle                |
| `COPROC`*   | `coproc`                              | processus parallèle (bash)                  |

_* certaines ne sont pas POSIX strict mais courantes en Bash_

---

### 2. Opérateurs de contrôle et de groupement

| Token     | Lexème | Description                        |
|-----------|--------|------------------------------------|
| `AND_IF`  | `&&`   | exécution conditionnelle « et »     |
| `OR_IF`   | `||`   | exécution conditionnelle « ou »     |
| `PIPE`    | `|`    | pipeline                           |
| `SEMI`    | `;`    | terminaison d’instruction          |
| `DSEMI`   | `;;`   | fin de clause dans `case`          |
| `AMP`     | `&`    | exécution en tâche de fond         |
| `LPAR`    | `(`    | début de subshell ou grouping      |
| `RPAR`    | `)`    | fin de subshell ou grouping        |
| `LBRACE`  | `{`    | début de bloc `{ … }`              |
| `RBRACE`  | `}`    | fin de bloc `{ … }`                |
| `DOUBLE_BRACKET_OPEN`*  | `[[`   | test conditionnel étendu (bash)    |
| `DOUBLE_BRACKET_CLOSE`* | `]]`   | fin du test étendu (bash)          |

---

### 3. Redirections d’I/O

| Token        | Lexème(s)      | Description                                 |
|--------------|----------------|---------------------------------------------|
| `IO_NUMBER`  | `[0-9]+` avant redir. | spécifie le descripteur (`2>`, `3<`)   |
| `LESS`       | `<`            | redirection stdin                           |
| `GREAT`      | `>`            | redirection stdout                          |
| `DLESS`      | `<<`           | here‑doc                                    |
| `DLESSDASH`  | `<<-`          | here‑doc avec tabulation ignorée            |
| `DGREAT`     | `>>`           | redirection append                          |
| `LESSAND`    | `<&`           | duplique un fd en lecture                   |
| `GREATAND`   | `>&`           | duplique un fd en écriture                  |
| `LESSGREAT`  | `<>`           | ouverture read/write                        |
| `CLOBBER`    | `>|`           | force overwrite (bash)                      |
| `HERESTRING`*| `<<<`          | here‑string (bash)                          |

---

### 4. Word parts (tokens atomiques lexicalement)

| Token              | Lexème                 | Contexte                         |
|--------------------|------------------------|----------------------------------|
| `LITERAL`          | suite de caractères    | hors opérateurs/méta/caractères  |
| `ESC_SEQ`          | `\` + `<car>`          | échappement d’un seul caractère  |
| `SQUOTE`           | `'`                    | début/fin de quote simple        |
| `DQUOTE`           | `"`                    | début/fin de quote double        |
| `BQUOTE_OPEN`      | `` ` ``                | début de backquote (bash)        |
| `BQUOTE_CLOSE`     | `` ` ``                | fin de backquote (bash)          |
| `PARAM_EXP_OPEN`   | `${`                   | début `${ … }`                   |
| `PARAM_EXP_CLOSE`  | `}`                    | fin `${ … }`                     |
| `CMD_SUBST_OPEN`   | `$(`                   | début `$( … )`                   |
| `CMD_SUBST_CLOSE`  | `)`                    | fin `$( … )`                     |
| `ARITH_EXP_OPEN`   | `$((`                  | début `$(( … ))`                 |
| `ARITH_EXP_CLOSE`  | `))`                   | fin `$(( … ))`                   |
| `DOLLAR`           | `$`                    | dollar isolé (début expansion)   |

> **Note :** tous ces tokens sont émis **bruts** par le lexer ; le parseur les agrège ensuite via  
> ```bnf
> word → word_part+
> ```  
> pour construire le non‑terminal `WORD`.

---

### 5. Divers

| Token     | Lexème          | Usage                               |
|-----------|-----------------|-------------------------------------|
| `NEWLINE` | fin de ligne    | termine une commande                |
| `EOF`     | fin d’entrée    | fin de saisie / script              |
| `COMMENT` | `#…<NL>`        | ligne ou fin de ligne commentée     |
| `WHITESPACE` | espaces, tab, `\r`, `\t` | souvent ignorés mais délimitent   |

---

Avec cette liste **archi‑complète**, ton lexer couvrira tous les éléments reconnus par la grammaire POSIX (et quelques extensions Bash courantes).