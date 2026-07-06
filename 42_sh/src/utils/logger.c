/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:25:51 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 20:37:17 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "utils.h"
#include "ft_std.h"
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

/**
 * @brief Initialise le logger.
 * @param lg Le logger à initialiser.
 */
void	log_init(t_logger *lg)
{
	memset(lg, 0, sizeof(t_logger));
}

/**
 * @brief Ajoute une chaîne de caractères au logger.
 * @param lg Le logger.
 * @param s La chaîne de caractères à ajouter.
 * @note Le logger est limité à LOG_BUF_CAP caractères.
 */
void	log_puts(t_logger *lg, const char *s)
{
	while (*s && lg->pos + 1 < LOG_BUF_CAP)
		lg->buf[lg->pos++] = *s++;
	lg->buf[lg->pos] = '\0';
}

/**
 * @brief Ajoute un caractère au logger.
 * @param lg Le logger.
 * @param c Le caractère à ajouter.
 * @note Le logger est limité à LOG_BUF_CAP caractères.
 */
void	log_putch(t_logger *lg, char c)
{
	if (lg->pos + 1 < LOG_BUF_CAP)
		lg->buf[lg->pos++] = c;
	lg->buf[lg->pos] = '\0';
}

/**
 * @brief Vide le logger.
 * @param lg Le logger à vider.
 */
void	log_flush(int fd, t_logger *lg, bool to_file)
{
	if (lg->pos && to_file == false)
		write(fd, lg->buf, lg->pos);
	#pragma region "DEBUG"
	if (lg->pos > 0 && to_file == true)
		debug_log(lg->buf);
	#pragma endregion
}

/**
 * @param lg Le logger.
 * @param n Le nombre à ajouter.
 */
void	log_putlong(t_logger *lg, long n)
{
	char	tmp[20];
	int		i;

	if (n < 0)
	{
		log_putch(lg, '-');
		if (n == LONG_MIN)
		{
			log_puts(lg, "9223372036854775808");
			return ;
		}
		n = -n;
	}
	if (n == 0)
	{
		return (log_putch(lg, '0'));
	}
	i = 0;
	while (n > 0)
	{
		tmp[i++] = '0' + (n % 10);
		n /= 10;
	}
	while (i--)
		log_putch(lg, tmp[i]);
}
