/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_squoted.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:12:33 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 06:45:59 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "module_token.h"
#include "module_lexer.h"
#include "module_input.h"

void	lex_until_squotes_end(t_lexer *lx)
{
	while (!is_squote(lx))
	{
		if (is_eof(lx))
			return ;
		advance_(&lx->inp);
	}
	erase_and_advance(&lx->inp);
	remove_matching_context(&lx->ctxs, CTX___SQUOTE);
	lx->pending_squote = false;
}

bool	lex_squotes_start(t_lexer *lx)
{
	if (lx->pending_dquote)
	{
		return (false);
	}
	if (is_squote(lx))
	{
		erase_and_advance(&lx->inp);
		lx->pending_squote = true;
		rem_op_context(lx);
		add_context(lx, TOK_SQUOTE);
		tk_temp_part_create(lx, TOK_SQUOTE);
		lex_until_squotes_end(lx);
		return (true);
	}
	return (false);
}
