# grammar/ - tables de dispatch

Point d'entrée unique pour **ajouter un opérateur ou une redirection**. Ici, tout
est déclaratif : une ligne de table par symbole. Pas de logique de lexer/parser/
exec, seulement les tables qui les relient.

## Ajouter un opérateur (`;`, `|`, `&&`, `&`, `;;`, ...)

1. `operators.c` : ajouter une ligne `{ "SYMBOLE", TOK_X, OPC_CONTROL }`.
2. `tok_to_ast.c` : ajouter `{ TOK_X, AST_X }`.
3. Écrire le handler d'exécution dans `../exec/control/` et l'enregistrer dans sa
   table `{ AST_X, handler }`.

## Ajouter une redirection (`>`, `<`, `>>`, `<<`, `<>`, `>|`, ...)

1. `operators.c` : ajouter `{ "SYMBOLE", TOK_REDIR_X, OPC_REDIR }`.
2. `../exec/redir/` : ajouter la ligne d'application `{ TOK_REDIR_X, open_flags, apply }`.

## Rappel du modèle

- Opérateur de **contrôle** (`;`, `|`, `&&`, `||`, `&`, `( )`) : structure l'arbre
  (devient un noeud AST, a un handler d'exécution).
- **Redirection** (`<`, `>`, ...) : attribut d'une commande, appliqué avant
  `execve` (open + dup2).

Une table incomplète se voit au premier test : c'est voulu.
