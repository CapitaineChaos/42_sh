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

char	*nano_get_next_line(int fd)
{
	t_strapi	api;
	char		*line;
	char		buf;
	ssize_t		r;

	strapi_init(1024, &api);
	while (1)
	{
		r = read(fd, &buf, 1);
		if (r < 0)
			return (api.free(&api), NULL);
		if (r == 0)
			break ;
		api.append_char(&api, buf);
		if (buf == '\n')
			break ;
	}
	line = api.get_dup(&api);
	api.free(&api);
	return (line);
}
