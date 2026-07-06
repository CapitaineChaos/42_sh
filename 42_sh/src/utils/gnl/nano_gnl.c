/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nano_gnl.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:44:24 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/20 17:00:42 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "ft_std.h"

char	*gnl_append(char *s, char c)
{
	int		i;
	char	*res;

	i = 0;
	while (s && s[i])
		i++;
	res = malloc(i + 2);
	if (!res)
		return (free(s), NULL);
	i = 0;
	while (s && s[i])
	{
		res[i] = s[i];
		i++;
	}
	res[i] = c;
	res[i + 1] = '\0';
	free(s);
	return (res);
}

char	*nano_get_next_line(int fd)
{
	char		*line;
	char		buf;
	ssize_t		r;

	line = NULL;
	while (1)
	{
		r = read(fd, &buf, 1);
		if (r < 0)
			return (free(line), NULL);
		if (r == 0)
			break ;
		line = gnl_append(line, buf);
		if (!line)
			return (NULL);
		if (buf == '\n')
			break ;
	}
	return (line);
}
