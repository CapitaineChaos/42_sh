/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wputchar_fd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:44:51 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/19 16:26:21 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>

static void	one_byte(int wc, char *buff, int *len)
{
	buff[0] = (char)wc;
	*len = 1;
}

static void	two_bytes(int wc, char *buff, int *len)
{
	buff[0] = (char)(0xC0 | ((wc >> 6) & 0x1F));
	buff[1] = (char)(0x80 | (wc & 0x3F));
	*len = 2;
}

static void	three_bytes(int wc, char *buff, int *len)
{
	buff[0] = (char)(0xE0 | ((wc >> 12) & 0x0F));
	buff[1] = (char)(0x80 | ((wc >> 6) & 0x3F));
	buff[2] = (char)(0x80 | (wc & 0x3F));
	*len = 3;
}

static void	four_bytes(int wc, char *buff, int *len)
{
	buff[0] = (char)(0xF0 | ((wc >> 18) & 0x07));
	buff[1] = (char)(0x80 | ((wc >> 12) & 0x3F));
	buff[2] = (char)(0x80 | ((wc >> 6) & 0x3F));
	buff[3] = (char)(0x80 | (wc & 0x3F));
	*len = 4;
}

int	ft_wputchar_fd(int wc, int fd)
{
	char	buff[4];
	int		len;

	len = 0;
	if (wc <= 0x7F)
		one_byte(wc, buff, &len);
	else if (wc <= 0x7FF)
		two_bytes(wc, buff, &len);
	else if (wc <= 0xFFFF)
		three_bytes(wc, buff, &len);
	else if (wc <= 0x10FFFF)
		four_bytes(wc, buff, &len);
	if (write(fd, buff, len) < 0)
		return (-1 * errno);
	else
		return (len);
}
