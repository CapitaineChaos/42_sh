/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_dquoted.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 14:34:14 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 08:09:41 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "module_token.h"
#include "module_lexer.h"
#include "module_input.h"

void	lex_until_dquotes_end(t_lexer *lx)
{
	while (!is_dquote(lx))
	{
		if (is_backslash(lx))
		{
			if (peek_next_chr(&lx->inp) != '\0')
				advance_(&lx->inp);
		}
		if (is_eof(lx))
		{
			return ;
		}
		advance_(&lx->inp);
	}
	erase_and_advance(&lx->inp);
	remove_matching_context(&lx->ctxs, CTX___DQUOTE);
	lx->pending_dquote = false;
}

bool	lex_dquotes_start(t_lexer *lx)
{
	if (lx->pending_squote)
		return (false);
	if (is_dquote(lx))
	{
		erase_and_advance(&lx->inp);
		lx->pending_dquote = true;
		rem_op_context(lx);
		add_context(lx, TOK_DQUOTE);
		tk_temp_part_create(lx, TOK_DQUOTE);
		lex_until_dquotes_end(lx);
		return (true);
	}
	return (false);
}
