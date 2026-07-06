/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inp_cmp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 02:51:47 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/17 01:43:00 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_input.h"
#include "ft_std.h"

bool	peek_chr_eq(t_input *inp, char c)
{
	if (!inp->stream)
		return (false);
	if (inp->stream[inp->pos] != c)
		return (false);
	return (true);
}

char	get_char(t_input *inp)
{
	if (!inp->stream)
		return (false);
	return (inp->stream[inp->pos]);
}

bool	check_char(t_input *inp, char c)
{
	if (!inp->stream)
		return (false);
	if (inp->stream[inp->pos] == c)
	{
		advance_(inp);
		return (true);
	}
	return (false);
}

bool	check_str(t_input *inp, char *str)
{
	size_t	i;

	if (!inp->stream)
		return (false);
	i = 0;
	while (str[i] && inp->stream[inp->pos + i] == str[i])
		i++;
	if (str[i] != '\0')
		return (false);
	inp->pos += i;
	return (true);
}

bool	peek_str_(t_input *inp, char *str)
{
	int		i;

	if (!inp->stream)
		return (false);
	i = 0;
	while (str[i] && inp->stream[inp->pos + i] == str[i])
		i++;
	if (str[i] != '\0')
		return (false);
	return (true);
}
