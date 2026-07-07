/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 01:50:25 by smaitre           #+#    #+#             */
/*   Updated: 2026/07/06 00:00:00 by codex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_input.h"
#include <string.h>

char	*get_ptr(t_input *inp)
{
	if (!inp->stream)
		return (NULL);
	return (inp->stream + inp->pos);
}

char	get_char(t_input *inp)
{
	if (!inp->stream)
		return ('\0');
	return (inp->stream[inp->pos]);
}

char	peek_next_chr(t_input *inp)
{
	if (!inp->stream)
		return ('\0');
	if (inp->stream[inp->pos + 1] == '\0')
		return ('\0');
	return (inp->stream[inp->pos + 1]);
}

bool	peek_chr_eq(t_input *inp, char c)
{
	if (!inp->stream)
		return (false);
	return (inp->stream[inp->pos] == c);
}

bool	peek_str_(t_input *inp, char *str)
{
	if (!inp->stream)
		return (false);
	return (strncmp(inp->stream + inp->pos, str, strlen(str)) == 0);
}

bool	peek_chars_2(t_input *inp, char *str)
{
	if (!inp->stream || inp->stream[inp->pos] == '\0')
		return (false);
	return (inp->stream[inp->pos] == str[0]
		&& inp->stream[inp->pos + 1] == str[1]);
}

void	advance_(t_input *inp)
{
	if (!inp->stream || inp->stream[inp->pos] == '\0')
		return ;
	inp->pos++;
}

bool	check_char(t_input *inp, char c)
{
	if (!peek_chr_eq(inp, c))
		return (false);
	advance_(inp);
	return (true);
}

bool	check_str(t_input *inp, char *str)
{
	size_t	len;

	if (!peek_str_(inp, str))
		return (false);
	len = strlen(str);
	inp->pos += len;
	return (true);
}
