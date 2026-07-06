/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:50:15 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/21 03:41:44 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_std.h"

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}

static void	ft_tolower_wrapper(unsigned int index, char *s)
{
	(void) index;
	*s = (char)ft_tolower(*s);
	return ;
}

void	ft_strtolower(char *str)
{
	ft_striteri(str, &ft_tolower_wrapper);
}
