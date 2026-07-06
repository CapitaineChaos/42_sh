/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:50:15 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/05 00:42:04 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_std.h"

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}

static void	ft_toupper_wrapper(unsigned int index, char *s)
{
	(void) index;
	*s = (char)ft_toupper(*s);
	return ;
}

void	ft_strtoupper(char *str)
{
	ft_striteri(str, &ft_toupper_wrapper);
}
