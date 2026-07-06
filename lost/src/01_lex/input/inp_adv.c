/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inp_adv.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 01:50:25 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/23 02:06:41 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_input.h"
#include "ft_std.h"

char	*get_ptr(t_input *inp)
{
	if (!inp->stream)
		return (false);
	return (inp->stream + inp->pos);
}

void	advance_(t_input *inp)
{
	if (!inp->stream)
		return ;
	if (inp->stream[inp->pos] == '\0')
		return ;
	inp->pos++;
}

void	erase_and_advance_str(t_input *inp, char *str)
{
	int		i;

	if (!inp->stream)
		return ;
	i = 0;
	while (str[i] && inp->stream[inp->pos + i] == str[i])
	{
		inp->stream[inp->pos + i] = '\0';
		i++;
	}
	inp->pos += i;
}

void	erase_and_advance(t_input *inp)
{
	if (!inp->stream)
		return ;
	if (inp->stream[inp->pos] == '\0')
		return ;
	inp->stream[inp->pos] = '\0';
	inp->pos++;
}
