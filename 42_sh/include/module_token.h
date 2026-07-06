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

typedef enum e_tk_kind
{
	TKD_OPERATOR,
	TKD_OPERAND,
	TKD_STRUCT,
	TKD_NOT_MANAGED,
}	t_tk_kind;

typedef enum e_tk_family
{
	TKF_WORD,
	TKF_OPERATOR,
	TKF_CONTROL,
	TKF_SUBSHELL,
	TKF_REDIRECT,
	TKF_WORD_REDIR,
	TKF_HEREDOC_BODY,
}	t_tk_family;

typedef enum e_tk_group
{
	TKG_ARGUMENT,
	TKG_REDIRECTION,
	TKG_GROUP,
	TKG_OTHER,
}	t_tk_group;

typedef enum e_tk_type
{
	// Generiques
	TOK_WORD,
	TOK_DELIM,
	TOK_OPERATOR,
	TOK_HEREDOC_BODY,
	// Mots (WORD PARTS)
	TOK_UQUOTE,				// "abc", 'echo', 'foo42', etc.
	TOK_DQUOTE,
	TOK_SQUOTE,
	TOK_GLOB,
	// Séparateurs & contrôle
	TOK_PIPE,				// |
	TOK_AND_IF,				// &&
	TOK_OR_IF,				// ||
	TOK_LPAREN,				// (
	TOK_RPAREN,				// )
	TOK_SEMI,				// ;
	TOK_ESCAPE,
	// Redirections
	TOK_REDIR_IN,			// <
	TOK_REDIR_OUT,			// >
	TOK_REDIR_APPEND,		// >>
	TOK_REDIR_HEREDOC,		// <<
	// Divers		
	TOK_EOF,				// fin du flux
	TOK_NEWLINE,			//
}	t_tk_type;

typedef struct s_tk_part
{
	t_tk_type			type;
	struct s_tk_part	*next;
	struct s_tk_part	*prev;
	char				*stream;
	char				*str;
	size_t				offset;
	bool				has_tilde;
	bool				has_dollar;
	bool				has_glob;

}	t_tk_part;

typedef struct s_token
{
	t_tk_kind		kind;
	t_tk_group		group;
	t_tk_type		type;
	t_tk_family		family;
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
void		tk_temp_part_create(t_lexer *lx, t_tk_type type);
t_tk_part	*tk_part_new(t_tk_type type, size_t offset);

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
/**
 * LIFECYCLE
 */

t_token		*token_new(t_tk_type type);
void		free_token_parts(t_tk_part *first_part);
void		free_token(t_token *token);
void		free_token_list(t_tokens *tokens);
void		free_token_part(t_tk_part *part);

#endif
