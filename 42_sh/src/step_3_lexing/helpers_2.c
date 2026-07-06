/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 23:29:04 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 00:23:23 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "module_lexer.h"

bool	is_wsp(t_lexer *lx)
{
	if (lx->pending_dquote || lx->pending_squote || lx->pending_escape)
		return (false);
	if (!is_eof(lx) && strchr(lx->ifs, get_char(&lx->inp)))
		return (true);
	return (false);
}

bool	is_sep(t_lexer *lx)
{
	if (lx->pending_dquote || lx->pending_squote || lx->pending_escape)
		return (false);
	if (strchr(lx->ifs, lx->inp.stream[lx->inp.pos]))
		return (true);
	return (false);
}

bool	is_backtick(t_lexer *lx)
{
	if (peek_chr_eq(&lx->inp, '`'))
		return (true);
	return (false);
}

bool	is_dollar(t_lexer *lx)
{
	if (peek_chr_eq(&lx->inp, '$'))
		return (true);
	return (false);
}

bool	is_backslash(t_lexer *lx)
{
	if (peek_chr_eq(&lx->inp, '\\'))
		return (true);
	return (false);
}
