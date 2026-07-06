/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cputchar_fd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:08:38 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/05 00:36:43 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

ssize_t	ft_cputchar_fd(char c, int fd)
{
	return (write(fd, &c, 1));
}

ssize_t	ft_cputstrn_fd(char *s, size_t n, int fd)
{
	ssize_t	len;
	ssize_t	clen;
	size_t	i;

	len = 0;
	i = 0;
	if (!s)
		return (0);
	while (i < n)
	{
		clen = ft_cputchar_fd(s[i], fd);
		if (clen > 0)
			len++;
		i++;
	}
	return (len);
}

ssize_t	ft_cputnchar_fd(char c, size_t nb, int fd)
{
	size_t	i;
	ssize_t	len;
	ssize_t	clen;

	i = 0;
	len = 0;
	while (i < nb)
	{
		clen = write(fd, &c, 1);
		if (clen > 0)
			len++;
		i++;
	}
	return (len);
}
