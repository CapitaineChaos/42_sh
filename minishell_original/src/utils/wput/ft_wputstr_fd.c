/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wputstr_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 23:51:59 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/19 16:29:10 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_wput.h"

static void	decode_one_byte(char **s, int *wc)
{
	*wc = (unsigned char)(**s);
	(*s)++;
}

static void	decode_two_bytes(char **s, int *wc)
{
	*wc = ((unsigned char)(**s) & 0x1F);
	*wc = (*wc << 6) | (((unsigned char)(*s)[1]) & 0x3F);
	*s += 2;
}

static void	decode_three_bytes(char **s, int *wc)
{
	*wc = ((unsigned char)(**s) & 0x0F);
	*wc = (*wc << 6) | (((unsigned char)(*s)[1]) & 0x3F);
	*wc = (*wc << 6) | (((unsigned char)(*s)[2]) & 0x3F);
	*s += 3;
}

static void	decode_four_bytes(char **s, int *wc)
{
	*wc = ((unsigned char)(**s) & 0x07);
	*wc = (*wc << 6) | (((unsigned char)(*s)[1]) & 0x3F);
	*wc = (*wc << 6) | (((unsigned char)(*s)[2]) & 0x3F);
	*wc = (*wc << 6) | (((unsigned char)(*s)[3]) & 0x3F);
	*s += 4;
}

int	ft_wputstr_fd(char *s, int fd)
{
	unsigned char	c;
	int				wc;
	int				len;
	int				i;

	len = 0;
	while (*s)
	{
		c = (unsigned char)*s;
		if (c < 0x80)
			decode_one_byte(&s, &wc);
		else if ((c & 0xE0) == 0xC0)
			decode_two_bytes(&s, &wc);
		else if ((c & 0xF0) == 0xE0)
			decode_three_bytes(&s, &wc);
		else if ((c & 0xF8) == 0xF0)
			decode_four_bytes(&s, &wc);
		else
			wc = (unsigned char)*s++;
		i = ft_wputchar_fd(wc, fd);
		if (i < 0)
			return (i);
		len += i;
	}
	return (len);
}
