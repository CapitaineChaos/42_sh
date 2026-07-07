/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 23:29:04 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 00:23:23 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_lexer.h"

bool	is_newline(t_lexer *lx)
{
	return (peek_chr_eq(&lx->inp, '\n'));
}

bool	is_eof(t_lexer *lx)
{
	return (peek_chr_eq(&lx->inp, '\0'));
}

bool	is_dquote(t_lexer *lx)
{
	return (peek_chr_eq(&lx->inp, '"'));
}

bool	is_squote(t_lexer *lx)
{
	return (peek_chr_eq(&lx->inp, '\''));
}

bool	is_comment(t_lexer *lx)
{
	return (peek_chr_eq(&lx->inp, '#'));
}

bool	is_wsp(t_lexer *lx)
{
	if (lx->state != LEX_NORMAL)
		return (false);
	return (!is_eof(lx) && strchr(lx->ifs, get_char(&lx->inp)));
}

bool	is_sep(t_lexer *lx)
{
	if (lx->state != LEX_NORMAL)
		return (false);
	return (strchr(lx->ifs, lx->inp.stream[lx->inp.pos]));
}

bool	is_backtick(t_lexer *lx)
{
	return (peek_chr_eq(&lx->inp, '`'));
}

bool	is_dollar(t_lexer *lx)
{
	return (peek_chr_eq(&lx->inp, '$'));
}

bool	is_backslash(t_lexer *lx)
{
	return (peek_chr_eq(&lx->inp, '\\'));
}
