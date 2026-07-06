/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_title.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 20:06:22 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 20:37:17 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "colors.h"
#include "utils.h"
#include <unistd.h>
#include <stddef.h>

void	debug_nano_title(int LVL, char *title)
{
	t_logger	lg;
	int			title_len;
	int			i;
	int			max;

	if (DLVL < LVL || title == NULL)
		return ;
	title_len = strlen(title);
	max = 55;
	log_init(&lg);
	log_puts(&lg, "\n-");
	i = 0;
	while (i < (max - 8 - title_len) / 2)
	{
		log_putch(&lg, '~');
		i++;
	}
	log_puts(&lg, "|  ");
	i = i + 3;
	log_puts(&lg, title);
	i = i + title_len + 1;
	log_puts(&lg, "  |");
	i = i + 3;
	while (i < max - 1)
	{
		log_putch(&lg, '~');
		i++;
	}
	log_puts(&lg, "-\n\n");
	log_flush(-1, &lg, true);
}

void	debug_pico_title(int LVL, char *title, char *color)
{
	t_logger	lg;
	int			title_len;
	int			i;
	int			max;

	if (DLVL < LVL || title == NULL)
		return ;
	title_len = strlen(title);
	max = 55;
	log_init(&lg);
	log_puts(&lg, "x");
	i = 0;
	while (i < (max - 8 - title_len) / 2)
	{
		log_putch(&lg, '-');
		i++;
	}
	if (color)
		log_puts(&lg, color);
	log_puts(&lg, "[  ");
	i = i + 3;
	log_puts(&lg, title);
	i = i + title_len + 1;
	log_puts(&lg, "  ]");
	if (color)
		log_puts(&lg, RT);
	i = i + 3;
	while (i < max - 1)
	{
		log_putch(&lg, '-');
		i++;
	}
	log_puts(&lg, "x\n");
	log_flush(-1, &lg, true);
}


void	debug_mini_title(int LVL, char *title)
{
	t_logger	lg;
	int			title_len;
	int			i;
	int			max;

	if (DLVL < LVL || title == NULL)
		return ;
	title_len = strlen(title);
	max = 55;
	log_init(&lg);
	log_puts(&lg, "\no");
	i = 0;
	while (i < (max - 8 - title_len) / 2)
	{
		log_putch(&lg, '-');
		i++;
	}
	log_puts(&lg, "[  ");
	i = i + 3;
	log_puts(&lg, title);
	i = i + title_len + 1;
	log_puts(&lg, "  ]");
	i = i + 3;
	while (i < max - 1)
	{
		log_putch(&lg, '-');
		i++;
	}
	log_puts(&lg, "o\n\n");
	log_flush(-1, &lg, true);
}

void	debug_title(int LVL, char *title)
{
	t_logger	lg;
	int			title_len;
	int			i;
	int			max;

	if (DLVL < LVL || title == NULL)
		return ;
	title_len = strlen(title);
	max = 55;
	log_init(&lg);
	log_puts(&lg, "\n");
	i = 0;
	while (i < max)
	{
		log_putch(&lg, '=');
		i++;
	}
	log_puts(&lg, "\n=");
	i = 1;
	while (i < (max - 2 - title_len) / 2)
	{
		log_putch(&lg, ' ');
		i++;
	}
	log_puts(&lg, title);
	i = i + title_len + 1;
	while (i < max)
	{
		log_putch(&lg, ' ');
		i++;
	}
	log_puts(&lg, "=\n");
	i = 0;
	while (i < max)
	{
		log_putch(&lg, '=');
		i++;
	}
	log_puts(&lg, "\n\n");
	log_flush(-1, &lg, true);
}
