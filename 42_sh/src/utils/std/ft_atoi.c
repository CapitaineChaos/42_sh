/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 11:36:14 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/17 21:27:08 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_std.h"

int	ft_atoi_secure(const char *nptr, bool *error)
{
	int		signe;
	long	nb;

	signe = 1;
	nb = 0;
	*error = false;
	while (isspace((unsigned char)*nptr))
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			signe = -1;
		nptr++;
	}
	while (*nptr && isdigit((unsigned char)*nptr))
	{
		nb = nb * 10 + (*nptr++ - '0');
		if (signe == 1 && nb > 2147483647)
			*error = true;
		if (signe == -1 && nb > 2147483648)
			*error = true;
	}
	if (*error == true)
		return (0);
	return ((int)(signe * nb));
}
