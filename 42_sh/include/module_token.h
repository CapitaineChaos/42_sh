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

/* Classe lexicale : partition, une valeur par token. Lue via tok_class(). */
typedef enum e_tk_class
{
	LCL_WORD,
	LCL_CONTROL,
	LCL_REDIR,
	LCL_INTERNAL,
}	t_tk_class;

/* Attributs non exclusifs, combinables en masque. Testés via tok_has(). */
typedef enum e_tk_attr
{
	TA_NONE = 0,
	TA_CMD_PART = 1 << 0,
	TA_SEP = 1 << 1,
	TA_BINARY = 1 << 2,
	TA_ANDOR = 1 << 3,
	TA_REDIR_IN = 1 << 4,
	TA_REDIR_OUT = 1 << 5,
	TA_REDIR_HERE = 1 << 6,
}	t_tk_attr;

typedef struct s_tk_spec
{
	const char	*name;
	const char	*lexeme;
	t_tk_class	cls;
	int			attrs;
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
bool		tok_has(t_tk_type type, int attrs);
t_tk_class	tok_class(t_tk_type type);
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
