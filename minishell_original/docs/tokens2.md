Voici la liste complète des **tokens majeurs** reconnus par un lexer Bash conforme à POSIX & extensions Bash, puis des **sous‑tokens** (ou *word parts*) qui composent un `WORD` :

---

**1. Tokens majeurs**  
- **WORD**  
- **ASSIGNMENT_WORD** (mot contenant `=` pour une affectation)  
- **NAME** (identificateur pur pour commande ou fonction)  
- **IO_NUMBER** (`[0-9]+` immédiatement suivi d’une redirection)  
- **MOTS RÉSERVÉS**  
  - `if`, `then`, `else`, `elif`, `fi`  
  - `case`, `esac`  
  - `for`, `select`, `while`, `until`, `do`, `done`, `in`  
  - `function`, `time`, `{`, `}`  
- **OPÉRATEURS ET PONCTUATEURS**  
  - Séparateurs de commandes : `;` (SEMICOLON), `;;` (DSEMI), `\n` (NEWLINE)  
  - Exécution en arrière‑plan : `&` (AMP), `&&` (AND_IF)  
  - Pipes : `|` (PIPE), `||` (OR_IF)  
  - Redirections classiques : `<` (LESS), `>` (GREAT), `<<` (DLESS), `>>` (DGREAT), `<<<` (HERESTR)  
  - Redirections avancées : `<&` (LESSAND), `>&` (GREATAND), `<>` (LESSGREAT), `>|` (CLOBBER)  
  - Groupement/subshell : `(` (LPAREN), `)` (RPAREN)  
  - Bloc `{…}` (reconnu comme mot réservé `{` puis `}`)  
- **FIN DE FICHIER** : `EOF`

---

**2. Sous‑tokens (*word parts*) d’un token WORD**  
Chaque `WORD` émis agrège une séquence ordonnée de ces fragments :  

- **LITERAL**  
  Suite de caractères non spéciaux, non quotés.  
- **SINGLE_QUOTED**  
  Contenu entre apostrophes simples (`'…'`).  
- **DOUBLE_QUOTED**  
  Contenu entre guillemets doubles (`"…"`), pouvant inclure d’autres parties.  
- **ESCAPE**  
  Caractère précédé d’un antislash (`\x`).  
- **TILDE_EXP**  
  Expansion de `~` ou `~user` en tête de mot.  
- **PARAM_EXP**  
  `$var`, `${var}`, `${var:-word}`, etc.  
- **ARITH_EXP**  
  `$(( expression ))` (arithmétique).  
- **CMD_SUBST**  
  `$( command )` ou `` `command` ``.  
- **PROC_SUBST**  
  `<( command )` ou `>( command )`.  
- **GLOB**  
  Métacaractères de motif : `*`, `?`, `[...]`.  
- **EXTGLOB**  
  Glob avancé : `@(pat)`, `*(pat)`, `?(pat)`, `+(pat)`, `!(pat)`.  
- **HEREDOC_BODY**  
  Contenu d’un here‑document après `<< word`.  

---

En pratique, ton lexer accumule ces **sous‑tokens** dans une liste `parts` tant que tu n’as pas atteint un séparateur (espace, opérateur, fin de ligne…). À ce moment tu émets **un seul** token `WORD` dont `parts` référence exactement la séquence collectée.