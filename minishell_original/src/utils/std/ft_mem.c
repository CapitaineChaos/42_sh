/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mem.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 14:27:16 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/07 20:50:58 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_std.h"
#include <stddef.h>
#include <stdlib.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*new_dst;
	unsigned char	*new_src;

	if (src == NULL && dest == NULL)
		return (NULL);
	if (n == 0)
		return (dest);
	new_dst = (unsigned char *)dest;
	new_src = (unsigned char *)src;
	while (n--)
		*(new_dst++) = *(new_src++);
	return (dest);
}

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*new_s;

	new_s = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		new_s[i] = (unsigned char)c;
		i++;
	}
	return (s);
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*new_dst;
	unsigned char	*new_src;

	if (dest > src)
	{
		new_dst = (unsigned char *) dest + n;
		new_src = (unsigned char *) src + n;
		while (n--)
			*(--new_dst) = *(--new_src);
	}
	else
		ft_memcpy(dest, src, n);
	return (dest);
}

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	if (nmemb != 0 && size > __SIZE_MAX__ / nmemb)
		return (NULL);
	ptr = malloc(nmemb * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, nmemb * size);
	return (ptr);
}
