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
#include "ft_strapi.h"

/* Lecture octet par octet (obligatoire sur un fd partagé : ne jamais lire
   au-delà du '\n'), accumulation O(n) amortie via exstr. */
char	*nano_get_next_line(int fd)
{
	t_strapi	line;
	char		buf;
	ssize_t		r;

	strapi_init(64, &line);
	while (1)
	{
		r = read(fd, &buf, 1);
		if (r < 0)
			return (exstr_free(&line), NULL);
		if (r == 0)
			break ;
		exstr_append_char(&line, buf);
		if (buf == '\n')
			break ;
	}
	if (line.data.len == 0)
		return (exstr_free(&line), NULL);
	return (line.data.str);
}
