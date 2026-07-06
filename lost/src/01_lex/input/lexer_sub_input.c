/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_sub_input.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 02:53:29 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/17 01:49:05 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_input.h"
#include "module_lexer.h"

bool	match_str(t_input *inp, char *str)
{
	size_t	len;
	size_t	i;

	if (!inp->stream)
		return (false);
	len = 0;
	while (str[len] && inp->stream[inp->pos + len]
		&& inp->stream[inp->pos + len] == str[len])
		len++;
	if (str[len] != '\0')
		return (false);
	i = 0;
	while (i < len)
	{
		inp->stream[inp->pos + i] = '\0';
		i++;
	}
	inp->pos += len;
	return (true);
}

bool	peek_chars_2(t_input *inp, char *str)
{
	if (!inp->stream)
		return (false);
	if (inp->stream[inp->pos] == '\0')
		return (false);
	if (inp->stream[inp->pos] == str[0]
		&& inp->stream[inp->pos + 1] == str[1])
		return (true);
	return (false);
}

char	peek_next_chr(t_input *inp)
{
	if (!inp->stream)
		return (false);
	if (inp->stream[inp->pos + 1] == '\0')
		return ('\0');
	return (inp->stream[inp->pos + 1]);
}
