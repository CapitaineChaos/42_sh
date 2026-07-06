/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_tokenize_struct.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 15:41:18 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/22 22:07:46 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_token.h"
#include "module_lexer.h"
#include "module_input.h"
#include "module_context.h"

static bool	match_st(t_lexer *lx, char *sk, t_tk_type t)
{
	if (peek_str_(&lx->inp, sk))
	{
		tk_word_emit(lx);
		erase_and_advance_str(&lx->inp, sk);
		tk_struct_emit(lx, sk, t);
		return (true);
	}
	return (false);
}

bool	lex_struct(t_lexer *lx)
{
	if (match_st(lx, "(", TOK_LPAREN))
	{
		add_context(lx, TOK_LPAREN);
		return (true);
	}
	if (match_st(lx, ")", TOK_RPAREN))
	{
		rem_sub_context(lx);
		rem_op_context(lx);
		return (true);
	}
	return (false);
}
