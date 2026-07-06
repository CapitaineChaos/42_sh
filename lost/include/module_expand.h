/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_expand.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 22:34:34 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 21:05:21 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODULE_EXPAND_H
# define MODULE_EXPAND_H

# include "module_token.h"
# include "module_ast.h"
# include "module_normalize.h"
# include "ft_strapi.h"
# include "ft_std.h"
# include "utils.h"
# include <stdbool.h>
# include <dirent.h>

typedef struct s_ast_node	t_ast_node;

char		*aggregate_final_token_parts(t_tk_part *first);
void		aggregate_token_list(t_token *first);
char		*aggregate_cmdline(t_tokens *cmd, t_tokens *redir);

void		tag_tokens_tildes(t_token *first);
void		tag_tokens_vars(t_token *first);
void		tag_tokens_globs(t_token *first);

bool		part_needs_split(t_tk_part *part);
char		*dup_word(char *start, int len);
bool		repl_and_free_str(t_tk_part *part, char *new_str);
void		copy_char(t_strapi *out, char *s, size_t *i);
void		*return_and_free(t_strapi *out);
int			return_and_send_error(int n);
int			find_unescaped_char(char *str, char c);

int			expand_redirs(t_ast_node *node);
void		expand_args(t_ast_node *node);

char		*tilde_expand_word(const char *w);

t_token		*get_globs(t_tokens *toks, int *n, t_token **cur);
char		*get_dir_path(const char *pattern);
const char	*get_file_pattern(const char *pattern);
t_token		*expand_wildcards(int *count, const char *pattern);
void		insert_sorted(t_token **head, t_token *new);
int			add_matches_from_dir(t_token **result, DIR *dir, char *dir_path,
				const char *file_pattern);

char		*expand_heredoc(char *src, bool has_delim);
void		expand_heredoc_vars(t_tokens *tokens);

char		*fetch_special_var(char *s, size_t *i);
bool		process_dollar(t_strapi *out, char *s, size_t *i);
char		*expand_dollar(char *src);

void		expand_heredoc_vars(t_tokens *tokens);
void		expand_redir_vars(t_tokens *tokens);
void		expand_cmd_vars(t_tokens *tokens);

int			expand_cmd_tildes(t_tokens *lst);
bool		expand_cmd_wildcards(t_tokens *tokens);
int			expand_redir_wildcards(t_tokens *tokens);
int			expand_redir_tildes(t_tokens *tokens);

bool		is_splittable_part(t_tk_part *part);;
int			is_unescaped_delim(char *str, int pos, char *delims);
int			split_tokens(t_tokens *tokens, bool is_redir);
t_tokens	split_parts(t_token *tok);
char		**split_unescaped(char *str, char *delims, int *nb);
void		free_split(char **split);

#endif
