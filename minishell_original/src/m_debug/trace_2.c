/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 15:26:08 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 20:37:17 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_config.h"
#include "module_debug.h"
#include "colors.h"
#include "utils.h"

void	trace_short_info(int lv, char *str)
{
	t_logger	lg;

	if (DLVL < lv || str == NULL)
		return ;
	log_init(&lg);
	log_puts(&lg, BBL "[");
	log_puts(&lg, convert_dbg_lvl_name(lv));
	log_puts(&lg, BBL "]    ");
	log_puts(&lg, LGR);
	log_puts(&lg, str);
	log_puts(&lg, RT "\n");
	log_flush(-1, &lg, true);
}

void	trace_short_info_no_nl(int lv, char *str)
{
	t_logger	lg;

	if (DLVL < lv || str == NULL)
		return ;
	log_init(&lg);
	log_puts(&lg, str);
	log_flush(-1, &lg, true);
}

void	trace_info_nvchr(int lv, char *name, char c)
{
	t_logger	lg;

	if (DLVL < lv || name == NULL)
		return ;
	log_init(&lg);
	log_puts(&lg, BBL "[");
	log_puts(&lg, convert_dbg_lvl_name(lv));
	log_puts(&lg, BBL "] ");
	log_puts(&lg, LGR " > " CY "[INFO]" RT "     ");
	log_puts(&lg, name);
	log_puts(&lg, " : ");
	log_putch(&lg, c);
	log_puts(&lg, RT "\n");
	log_flush(-1, &lg, true);
}

void	trace_info_nvstr(int lv, char *name, char *value)
{
	t_logger	lg;

	if (DLVL < lv || name == NULL)
		return ;
	log_init(&lg);
	log_puts(&lg, BBL "[");
	log_puts(&lg, convert_dbg_lvl_name(lv));
	log_puts(&lg, BBL "] ");
	log_puts(&lg, LGR " > " CY "[INFO]" RT "     ");
	log_puts(&lg, name);
	log_puts(&lg, " : ");
	if (value == NULL)
	{
		log_puts(&lg, "NULL");
	}
	else
	{
		log_puts(&lg, value);
	}
	log_puts(&lg, RT "\n");
	log_flush(-1, &lg, true);
}

void	trace_info_nvnb(int lv, char *name, int value)
{
	t_logger	lg;

	if (DLVL < lv || name == NULL)
		return ;
	log_init(&lg);
	log_puts(&lg, BBL "[");
	log_puts(&lg, convert_dbg_lvl_name(lv));
	log_puts(&lg, BBL "] ");
	log_puts(&lg, LGR " > " CY "[INFO]" RT "  📌 ");
	log_puts(&lg, name);
	log_puts(&lg, " : ");
	log_putlong(&lg, value);
	log_puts(&lg, RT "\n");
	log_flush(-1, &lg, true);
}
