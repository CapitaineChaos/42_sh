/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_max.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 14:28:54 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/05 00:37:33 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

ssize_t	ft_max(ssize_t a, ssize_t b)
{
	if (a <= b)
		return (b);
	return (a);
}

ssize_t	ft_min(ssize_t a, ssize_t b)
{
	if (a >= b)
		return (b);
	return (a);
}
