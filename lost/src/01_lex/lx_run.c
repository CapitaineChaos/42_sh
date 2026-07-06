/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_run.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:13:58 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 21:27:42 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "module_token.h"
#include "module_lexer.h"

static void	manage_pendings(t_lexer *lx)
{
	rem_esc_context(lx);
	lx->pending_escape = false;
	if (lx->pending_dquote)
		lex_until_dquotes_end(lx);
	else if (lx->pending_squote)
		lex_until_squotes_end(lx);
}

void	do_lexing(t_lexer *lx)
{
	while (!is_eof(lx))
	{
		if (lex_squotes_start(lx))
			continue ;
		if (lex_dquotes_start(lx))
			continue ;
		if (lex_operator(lx))
			continue ;
		if (lex_redirection(lx))
			continue ;
		if (lex_struct(lx))
			continue ;
		lex_unquoted(lx);
	}
}

void	run_lexer(t_lexer *lx, int lv)
{
	if (lv < 3)
		return ;
	lx->heredoc_count = 0;
	if (is_eof(lx))
		return ;
	manage_pendings(lx);
	do_lexing(lx);
	if (lx->pending_escape)
	{
		add_context(lx, TOK_ESCAPE);
	}
	else if (!lx->pending_squote && !lx->pending_dquote)
	{
		tk_word_emit(lx);
		if (isatty(STDIN_FILENO))
			tk_control_emit(lx, "newline", TOK_NEWLINE);
		if (lx->ctxs.count == 0)
		{
			tk_control_emit(lx, "eof", TOK_EOF);
		}
	}
}
