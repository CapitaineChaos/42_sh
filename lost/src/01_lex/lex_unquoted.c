/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_unquoted.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:13:03 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 00:17:59 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "module_token.h"
#include "module_lexer.h"
#include "module_input.h"

static bool	handle_backslash(t_lexer *lx)
{
	if (is_backslash(lx))
	{
		lx->pending_escape = true;
		advance_(&lx->inp);
		if (!is_eof(lx))
		{
			if (lx->pending_escape)
			{
				lx->pending_escape = false;
			}
			advance_(&lx->inp);
		}
		return (true);
	}
	return (false);
}

static bool	handle_comment(t_lexer *lx)
{
	if (!(lx->pending_dquote || lx->pending_squote) || lx->pending_escape)
	{
		if (is_comment(lx))
		{
			tk_word_emit(lx);
			if (lx->pending_escape)
			{
				lx->pending_escape = false;
			}
			erase_and_advance(&lx->inp);
			while (!is_eof(lx) && !is_backslash(lx))
				advance_(&lx->inp);
			return (true);
		}
	}
	return (false);
}

void	lex_until_unquoted_end(t_lexer *lx)
{
	bool	had_char;

	while (!is_eof(lx))
	{
		if (lex_operator(lx) || lex_redirection(lx) || lex_struct(lx))
		{
			if (had_char)
				tk_word_emit(lx);
			return ;
		}
		if (is_sep(lx) || is_wsp(lx) || is_dquote(lx) || is_squote(lx))
		{
			return ;
		}
		if (handle_backslash(lx))
		{
			advance_(&lx->inp);
			continue ;
		}
		advance_(&lx->inp);
		had_char = true;
	}
}

static void	skip_spaces(t_lexer *lx, bool *had_sep)
{
	while (is_wsp(lx))
	{
		if (!*had_sep)
		{
			tk_word_emit(lx);
			*had_sep = true;
		}
		if (is_newline(lx))
		{
			tk_control_emit(lx, "newline", TOK_NEWLINE);
		}
		erase_and_advance(&lx->inp);
	}
}

void	lex_unquoted(t_lexer *lx)
{
	bool	had_sep;

	if (is_eof(lx))
	{
		if (lx->pending_escape)
		{
			lx->pending_escape = false;
		}
		return ;
	}
	had_sep = false;
	skip_spaces(lx, &had_sep);
	if (had_sep)
		return ;
	if (handle_comment(lx))
		return ;
	if (!is_eof(lx))
		tk_temp_part_create(lx, TOK_UQUOTE);
	rem_op_context(lx);
	lex_until_unquoted_end(lx);
}
