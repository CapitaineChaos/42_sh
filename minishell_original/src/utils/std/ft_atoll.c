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

static bool	is_overflow_pos(intmax_t acc, int digit)
{
	return (acc > (INTMAX_MAX - digit) / 10);
}

static bool	is_overflow_neg(intmax_t acc, int digit)
{
	return (acc < (INTMAX_MIN + digit) / 10);
}

static bool	is_overflow(intmax_t acc, const char *s, int sign, bool *error)
{
	int	d;

	d = *s - '0';
	if ((sign == 1 && is_overflow_pos(acc, d))
		|| (sign == -1 && is_overflow_neg(-acc, d)))
	{
		*error = true;
		return (true);
	}
	return (false);
}

static void	process_sign(const char **s, int *sign)
{
	if (**s == '+' || **s == '-')
	{
		if (**s == '-')
			*sign = -1;
		(*s)++;
	}
}

intmax_t	ft_atoll_secure(const char *s, bool *error)
{
	int			sign;
	intmax_t	acc;

	*error = false;
	sign = 1;
	acc = 0;
	while (ft_isspace((unsigned char)*s))
		++s;
	process_sign(&s, &sign);
	if (!ft_isdigit((unsigned char)*s))
	{
		*error = true;
		return (0);
	}
	while (ft_isdigit((unsigned char)*s))
	{
		if (is_overflow(acc, s, sign, error))
			return (0);
		acc = acc * 10 + *s - '0';
		++s;
	}
	return (sign * acc);
}
