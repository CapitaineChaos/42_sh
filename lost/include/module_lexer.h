/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_lexer.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 02:28:27 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 00:21:55 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODULE_LEXER_H
# define MODULE_LEXER_H

# include "module_token.h"
# include "module_context.h"
# include "module_checker.h"
# include "module_input.h"
# include "utils.h"
# include <stddef.h>
# include <stdbool.h>

struct						s_mns;
typedef struct s_mns		t_mns;

typedef struct s_lexer
{
	char			*ifs;
	t_tokens		tokens;
	t_input			inp;
	t_tk_part		*tmp_wp;
	t_contexts		ctxs;
	bool			pending_squote;
	bool			pending_dquote;
	bool			pending_escape;
	t_sstr			*heredoc_garbage;
	t_sstr			*garbage;
	int				heredoc_count;
}	t_lexer;

void	run_lexer(t_lexer *lx, int lv);

void	lexer_init(t_lexer *lx, char *ifs, int lv);
void	lexer_free(t_lexer *lx, int lv);
void	lexer_soft_reset(t_lexer *lx, int lv);
void	lexer_hard_reset(t_lexer *lx, int lv);

bool	is_wsp(t_lexer *lx);
bool	is_sep(t_lexer *lx);
bool	is_dquote(t_lexer *lx);
bool	is_squote(t_lexer *lx);
bool	is_eof(t_lexer *lx);

bool	is_newline(t_lexer *lx);
bool	is_dollar(t_lexer *lx);
bool	is_backslash(t_lexer *lx);
bool	is_backtick(t_lexer *lx);
bool	is_comment(t_lexer *lx);

bool	lex_operator(t_lexer *lx);
bool	lex_redirection(t_lexer *lx);
bool	lex_struct(t_lexer *lx);

void	lex_unquoted(t_lexer *lx);
bool	lex_squotes(t_lexer *lx);

void	lex_until_unquoted_end(t_lexer *lx);
void	lex_until_dquotes_end(t_lexer *lx);
bool	lex_dquotes_start(t_lexer *lx);
void	lex_until_squotes_end(t_lexer *lx);
bool	lex_squotes_start(t_lexer *lx);

void	free_and_exit_minishell(intmax_t exit_code);

#endif
