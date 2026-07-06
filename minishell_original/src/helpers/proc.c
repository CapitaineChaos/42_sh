/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 18:02:19 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/18 17:04:40 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include "ft_std.h"

static int	ft_rd(const char *path, char *buf, size_t size, ssize_t *out_len)
{
	int		fd;
	ssize_t	n;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (-1);
	n = read(fd, buf, size - 1);
	close(fd);
	if (n <= 0)
		return (-1);
	buf[n] = '\0';
	if (out_len)
		*out_len = n;
	return (0);
}

int	get_uid_from_proc(void)
{
	char		buf[16];
	ssize_t		n;

	if (ft_rd("/proc/self/loginuid", buf, sizeof(buf), &n) < 0)
		return (-1);
	return (ft_atoi(buf));
}

int	get_fd_soft_limit_from_proc(void)
{
	char		buf[2048];
	ssize_t		n;
	char		*start;

	if (ft_rd("/proc/self/limits", buf, sizeof(buf), &n) < 0)
		return (1024);
	start = buf;
	while (start && *start)
	{
		if (ft_strncmp(start, "Max open files", 14) == 0)
		{
			while (*start && (*start < '0' || *start > '9'))
				start++;
			return (ft_atoi(start));
		}
		while (*start && *start != '\n')
			start++;
		if (*start == '\n')
			start++;
	}
	return (1024);
}
