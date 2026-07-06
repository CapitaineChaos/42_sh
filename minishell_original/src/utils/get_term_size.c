/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_term_size.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 19:58:39 by smaitre           #+#    #+#             */
/*   Updated: 2025/04/29 19:59:07 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/ioctl.h>
#include <unistd.h>

void	get_terminal_size(int *cols, int *rows)
{
	struct winsize	ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
	{
		*cols = 80;
		*rows = 0;
		return ;
	}
	*cols = ws.ws_col;
	*rows = ws.ws_row;
	return ;
}
