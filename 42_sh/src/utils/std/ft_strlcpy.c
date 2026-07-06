/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:30:28 by smaitre           #+#    #+#             */
/*   Updated: 2025/04/29 20:03:48 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_std.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t siz)
{
	size_t	src_len;
	size_t	nb;

	src_len = strlen(src);
	if (siz > 0)
	{
		nb = src_len;
		if (siz < src_len + 1)
			nb = siz - 1;
		memcpy(dest, src, nb);
		dest[nb] = '\0';
	}
	return (src_len);
}
