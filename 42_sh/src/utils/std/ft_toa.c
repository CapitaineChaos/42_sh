/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toa.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:59:46 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/05 00:44:27 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_std.h"
#include <stdlib.h>

char	*ft_ltoa(long n)
{
	char	*s;
	size_t	len;
	int		sgn;

	sgn = 1;
	len = ft_nblen((size_t)n, 10);
	s = (char *)malloc((len + 1) * sizeof (char));
	if (!s)
		return (NULL);
	if (n < 0)
	{
		s[0] = '-';
		sgn = -1;
	}
	s[len] = '\0';
	if (n == 0)
		s[len - 1] = '0';
	while (n != 0)
	{
		s[len - 1] = (char)(sgn * (n % 10)) + '0';
		len--;
		n = n / 10;
	}
	return (s);
}

char	*ft_utoa_base(size_t nb, char *base)
{
	size_t	len;
	size_t	base_len;
	char	*snum;

	base_len = strlen(base);
	if (base_len < 2)
		return (NULL);
	len = ft_unblen(nb, base_len);
	snum = (char *)malloc((len + 1) * sizeof (char));
	if (!snum)
		return (NULL);
	snum[len] = '\0';
	while (len--)
	{
		snum[len] = base[nb % base_len];
		nb = nb / base_len;
	}
	return (snum);
}

char	*ft_itoa(int n)
{
	char	*s;
	int		len;
	int		sgn;

	sgn = 1;
	len = get_nblen(n);
	s = (char *)malloc((len + 1) * sizeof (char));
	if (!s)
		return (NULL);
	if (n < 0)
	{
		s[0] = '-';
		sgn = -1;
	}
	s[len] = '\0';
	if (n == 0)
		s[len - 1] = '0';
	while (n != 0)
	{
		s[len - 1] = (sgn * (n % 10)) + '0';
		len--;
		n = n / 10;
	}
	return (s);
}
