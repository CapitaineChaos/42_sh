Pour que ton here-document soit pris en compte, il faut en deux temps :

1. **Au moment du lexing**, juste après avoir reconnu le `<<` et le mot-clé délimitant, tu passes en mode « collecte de body » et tu lis les lignes **immédiatement** jusqu’à ce que la ligne lue soit exactement ton délimiteur. Pendant cette lecture tu accumules chaque ligne (avec un saut de ligne) dans un tampon (par exemple un `t_strapi`). Dès que tu rencontres le délimiteur, tu appelles `tk_heredoc_body_emit(…)` pour émettre un token `TOK_HEREDOC_BODY` dont `.t_tk_data.hdbody.str` contient tout le texte du here-doc.  

   Concrètement, dans `try_tokenize_redirection` (ou dans ton handler `handle_heredoc`), après avoir vu `<<` et lu le token suivant (le delimiter), tu fais par exemple :
   ```c
   // pseudo-code dans la boucle du lexer
   if (match_str(lx, "<<"))
   {
       char *delim = read_next_word(lx);
       t_strapi body;
       strapi_init(&body);
       while (true)
       {
           char *line = get_next_input_line();  // via readline ou read()
           if (strcmp(line, delim) == 0)
               break;
           body.append(&body, line);
           body.append(&body, "\n");
       }
       free(delim);
       tk_heredoc_body_emit(lx, &body);
       strapi_free(&body);
   }
   ```
   À la sortie du lexer, tu auras dans ta liste de tokens :
   ```
   TOK_REDIR_HEREDOC("<<") , TOK_WORD(delimiter) , TOK_HEREDOC_BODY(texte…)
   ```

2. **Au moment de l’exécution**, lorsque tu parcours ton AST et que tu tombes sur un nœud `AST_HEREDOC`, tu dois créer un pipe, y écrire **tout** le contenu du `TOK_HEREDOC_BODY` (stocké précédemment) puis fermer l’extrémité écriture, et enfin rediriger le descripteur lecture du pipe sur `STDIN_FILENO` du processus qui exécutera la commande suivante. Par exemple, dans `exec_node` quand tu détectes un `AST_HEREDOC` :

   ```c
   if (node->type == AST_HEREDOC)
   {
       int fd[2];
       pipe(fd);
       // écrire tout le contenu
       write(fd[1],
             node->t_ast_data.operand.tokens.head
                  ->t_tk_data.hdbody.str.get(&…),
             strlen(…));
       close(fd[1]);
       dup2(fd[0], STDIN_FILENO);
       close(fd[0]);
       // puis exec_node(node->right) ou similaire
   }
   ```

**En résumé**  
- **Lexing** : dès la détection de `<<`, bascule en mode lecture de here-doc et construis un token `TOK_HEREDOC_BODY` contenant tout le texte.  
- **Exécution** : en traitant l’AST, intercepte `AST_HEREDOC`, crée un pipe, écris le contenu du token dans le pipe et redirige l’entrée standard du processus suivant vers le pipe.  

C’est à ces deux moments précis — **immédiatement après le lexing** du `<<` pour la collecte, puis **juste avant l’exécution** de la commande qui suit — que tu dois « remplir » et utiliser les données du here-document.



##### ===============================================================================================================

Voici un exemple plus consistant :

```sh
grep -RIn --color=auto 'pattern' /var/log --exclude=*.tmp <<EOF >results.log 2>>errors.log <input.txt
Erreur : debug
Fin du here-doc.
EOF
```

**Décomposition en tokens** (ordre d’apparition) :

| Type de token         | Valeur                              |
|-----------------------|-------------------------------------|
| WORD                  | `grep`                              |
| WORD                  | `-RIn`                              |
| WORD                  | `--color=auto`                      |
| WORD                  | `pattern`                           |
| WORD                  | `/var/log`                          |
| WORD                  | `--exclude=*.tmp`                   |
| REDIR_HEREDOC (`<<`)  | `<<`                                |
| WORD                  | `EOF`                               |
| HEREDOC_BODY          | `"Erreur : debug\nFin du here-doc.\n"` |
| REDIR_OUT (`>`)       | `>`                                 |
| WORD                  | `results.log`                       |
| REDIR_ERR_APPEND (`>>`)| `>>`                              |
| WORD                  | `errors.log`                        |
| REDIR_IN (`<`)        | `<`                                 |
| WORD                  | `input.txt`                         |

> Ici, le seul **nœud AST_CMD** contiendra, dans son champ `operand.tokens`, la séquence :  
> `WORD('grep') WORD('-RIn') WORD('--color=auto') WORD('pattern') WORD('/var/log') WORD('--exclude=*.tmp') REDIR_HEREDOC('<<') WORD('EOF') HEREDOC_BODY(...) REDIR_OUT('>') WORD('results.log') REDIR_ERR_APPEND('>>') WORD('errors.log') REDIR_IN('<') WORD('input.txt')`  

Le shell :  
1. collecte le here-doc jusqu’à ce qu’il trouve `EOF` en début de ligne,  
2. émet un token `TOK_HEREDOC_BODY` contenant tout le texte entre les délimiteurs,  
3. puis exécute `grep` avec ses arguments et redirections (stdin venant d’un pipe sur le here-doc, stdout vers `results.log`, stderr en append vers `errors.log`, et lecture initiale de `input.txt`).





Oui : dans la grammaire POSIX une redirection (`<`, `>`, `<<`, `>>`, etc.) **doit impérativement** être suivi d’un **WORD** (le nom de fichier ou le délimiteur de here-doc). Si ton parser rencontre un opérateur de redirection et que le token suivant n’est **pas** de type `TOK_WORD`, il doit déclencher une erreur de syntaxe.

---

### Exemple d’implémentation de la vérification

```c
bool	try_create_redirection(t_tokens *tokens, t_pstack *output)
{
    t_token *redir;
    t_token *target;

    if (tokens->head->tclass != TKC_REDIRECT)
        return (false);

    /* on extrait l’opérateur de redirection */
    redir = tk_list_pop_front(tokens);

    /* on regarde s’il reste un token et s’il s’agit d’un WORD */
    if (!tokens->head || tokens->head->type != TOK_WORD)
    {
        /* pas de fichier après la redirection → syntax error */
        syntax_error("unexpected token after redirection");
        free_token(redir);
        return (false);
    }

    /* sinon, on récupère le WORD cible */
    target = tk_list_pop_front(tokens);
    /* on construit un nœud AST_CMD et on y rattache redir+target */
    t_ast_node *cmd = peek_front(output);
    tk_list_append(&cmd->t_ast_data.operand.tokens, redir);
    tk_list_append(&cmd->t_ast_data.operand.tokens, target);
    return (true);
}
```

- **TKC_REDIRECT** couvre `<`, `>`, `<<`, `>>`.  
- Si `tokens->head` est `NULL` ou un autre type que `TOK_WORD`, on signale immédiatement une erreur syntaxique.  

Ainsi tu garantis qu’aucune redirection ne soit laissée orpheline, et que ton AST reste toujours cohérent.