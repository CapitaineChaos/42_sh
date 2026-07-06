/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_len.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 15:15:50 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/05 00:44:36 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_std.h"
#include <unistd.h>
#include <stddef.h>

int	get_nblen(int n)
{
	int	len;

	len = n <= 0;
	while (n != 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

size_t	ft_nblen(long long int nb, size_t base_len)
{
	if (nb == 0)
		return (1);
	if (nb < 0)
		return (ft_unblen(-1 * nb, base_len) + 1);
	return (ft_unblen(nb, base_len));
}

size_t	ft_unblen(unsigned long long int nb, size_t base_len)
{
	size_t	len;

	if (nb == 0)
		return (1);
	len = 0;
	while (nb)
	{
		nb = nb / base_len;
		len++;
	}
	return (len);
}

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}
