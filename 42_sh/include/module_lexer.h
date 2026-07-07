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

typedef enum e_lex_state
{
	LEX_NORMAL,
	LEX_SQUOTE,
	LEX_DQUOTE,
	LEX_ESCAPE,
}	t_lex_state;

typedef struct s_lexer
{
	char			*ifs;
	t_tokens		tokens;
	t_input			inp;
	t_tk_part		*tmp_wp;
	t_contexts		ctxs;
	t_lex_state	state;
	t_sstr			*heredoc_garbage;
	t_sstr			*garbage;
	int				heredoc_count;
}	t_lexer;

/**
 * MAIN LEXER FUNCTION
 */

void	run_lexer(t_lexer *lx, int lv);

/**
 * LEXER
 */

void	lexer_init(t_lexer *lx, char *ifs, int lv);
void	lexer_free(t_lexer *lx, int lv);
void	lexer_soft_reset(t_lexer *lx, int lv);
void	lexer_hard_reset(t_lexer *lx, int lv);

/**
 * TOKENIZATION
 */

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

/**
 * EXTERNAL PROTOTYPES
 */

void	free_and_exit_minishell(intmax_t exit_code);

#endif
