/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 21:26:35 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 21:20:54 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_std.h"
#include <errno.h>

/* Erreur si aucun chiffre lu (end == s) ou overflow (ERANGE). Le garbage en
   fin est ignoré, comme l'ancienne implémentation maison / atoll. */
intmax_t	ft_atoll_secure(const char *s, bool *error)
{
	char		*end;
	long long	val;

	errno = 0;
	val = strtoll(s, &end, 10);
	if (end == s || errno == ERANGE)
	{
		*error = true;
		return (0);
	}
	*error = false;
	return ((intmax_t)val);
}
