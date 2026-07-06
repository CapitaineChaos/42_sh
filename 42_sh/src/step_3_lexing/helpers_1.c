/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 21:52:33 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/22 20:38:11 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "module_lexer.h"

bool	is_newline(t_lexer *lx)
{
	if (peek_chr_eq(&lx->inp, '\n'))
		return (true);
	return (false);
}

bool	is_eof(t_lexer *lx)
{
	if (peek_chr_eq(&lx->inp, '\0'))
		return (true);
	return (false);
}

bool	is_dquote(t_lexer *lx)
{
	if (peek_chr_eq(&lx->inp, '"'))
		return (true);
	return (false);
}

bool	is_squote(t_lexer *lx)
{
	if (peek_chr_eq(&lx->inp, '\''))
		return (true);
	return (false);
}

bool	is_comment(t_lexer *lx)
{
	if (peek_chr_eq(&lx->inp, '#'))
		return (true);
	return (false);
}
