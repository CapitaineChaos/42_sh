/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_token.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 04:46:02 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 01:04:50 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODULE_TOKEN_H
# define MODULE_TOKEN_H

# include "ft_std.h"
# include <sys/types.h>
# include <stdbool.h>

struct					s_mns;
typedef struct s_mns	t_mns;
typedef struct s_lexer	t_lexer;

typedef enum e_tk_type
{
	TOK_WORD,
	TOK_PIPE,
	TOK_AND_IF,
	TOK_OR_IF,
	TOK_LPAREN,
	TOK_RPAREN,
	TOK_SEMI,
	TOK_ESCAPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_REDIR_APPEND,
	TOK_REDIR_HEREDOC,
	TOK_HEREDOC_BODY,
	TOK_EOF,
	TOK_NEWLINE,
	TOK_COUNT,
}	t_tk_type;

typedef enum e_tk_role
{
	TKR_NONE,
	TKR_ARGUMENT,
	TKR_REDIR_OP,
	TKR_REDIR_TARGET,
	TKR_HEREDOC_DELIM,
	TKR_HEREDOC_BODY,
}	t_tk_role;

typedef enum e_tk_part_type
{
	PART_UQUOTE,
	PART_DQUOTE,
	PART_SQUOTE,
	PART_GLOB,
}	t_tk_part_type;

typedef enum e_tk_flags
{
	TF_NONE = 0,
	TF_WORD = 1 << 0,
	TF_OPERAND = 1 << 1,
	TF_REDIR = 1 << 2,
	TF_CONTROL_OP = 1 << 3,
	TF_LIST_SEP = 1 << 4,
	TF_PIPE_OP = 1 << 5,
	TF_AND_OR_OP = 1 << 6,
	TF_SUBSHELL = 1 << 7,
	TF_TERMINATOR = 1 << 8,
	TF_SYNTHETIC = 1 << 9,
	TF_REDIR_INPUT = 1 << 10,
	TF_REDIR_OUTPUT = 1 << 11,
}	t_tk_flags;

typedef struct s_tk_spec
{
	const char	*name;
	const char	*lexeme;
	int			flags;
}	t_tk_spec;

typedef struct s_tk_part
{
	t_tk_part_type		type;
	struct s_tk_part	*next;
	struct s_tk_part	*prev;
	char				*str;
	size_t				start;
	size_t				end;
	bool				has_tilde;
	bool				has_dollar;
	bool				has_glob;

}	t_tk_part;

typedef struct s_token
{
	t_tk_type		type;
	t_tk_role		role;
	char			*source;
	char			*str;
	ssize_t			count;
	t_tk_part		*head;
	t_tk_part		*tail;
	struct s_token	*next;
	struct s_token	*prev;
	bool			tk_flag;
	bool			has_quoted_part;
	bool			has_dollar;
	bool			has_tilde;
	bool			has_glob;
}	t_token;

typedef struct s_tokens
{
	t_token	*head;
	t_token	*tail;
	int		count;
}	t_tokens;

/**
 * OP REDIR
 */

void		tk_heredoc_body_emit(t_lexer *lx, t_token *next, char *body);
void		tk_redir_emit(t_lexer *lx, char *str, t_tk_type type);
void		tk_struct_emit(t_lexer *lx, char *s, t_tk_type type);

/**
 * CONTROL / OPERATOR
 */

void		tk_control_emit(t_lexer *lx, char *s, t_tk_type type);
void		tk_operator_emit(t_lexer *lx, char *s, t_tk_type type);

/**
 * WORD
 */

t_token		*new_word_token(t_tk_part *first);
void		transplant_parts(t_token *token, t_tk_part *parts);
void		tk_word_emit(t_lexer *lx);
t_token		*tk_glob_emit(char *str);

/**
 * PARTS
 */

void		append_part_to_part(t_tk_part **first, t_tk_part *new);
t_tk_part	*pop_front_part(t_token *token);
void		tk_append_part_tok(t_token *token, t_tk_part *part);
void		tk_temp_part_create(t_lexer *lx, t_tk_part_type type);
t_tk_part	*tk_part_new(t_tk_part_type type, size_t offset);

/**
 * APPEND
 */

void		replace_in_list(t_tokens *tokens, t_token *old, t_token *new_head);
t_token		*tk_list_pop_front(t_tokens *tokens);
void		tk_list_append(t_tokens *tokens, t_token *tok);
void		tk_list_insert(t_tokens *tokens, t_token *after_me, t_token *tok);

/**
 * CONTENT
 */

char		*aggregate_wordparts_to_strline(t_tk_part *first, char *line);
char		*slice_dup(const char *src, size_t start, size_t end);
bool		tok_has(t_tk_type type, int flags);
const char	*tok_name(t_tk_type type);
const char	*part_name(t_tk_part_type type);
/**
 * LIFECYCLE
 */

t_token		*token_new(t_tk_type type);
void		free_token_parts(t_tk_part *first_part);
void		free_token(t_token *token);
void		free_token_list(t_tokens *tokens);
void		free_token_part(t_tk_part *part);

extern const t_tk_spec	g_tok_specs[TOK_COUNT];

#endif
