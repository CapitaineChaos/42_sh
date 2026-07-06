/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 06:47:52 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 20:37:17 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_config.h"
#include "module_debug.h"
#include "colors.h"
#include "utils.h"

void	trace_info_nvcharpos(int lv, char *name, int pos, char c)
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
	log_puts(&lg, " : [");
	log_putlong(&lg, pos);
	log_puts(&lg, "] ");
	log_puts(&lg, "    -   ");
	log_putch(&lg, c);
	log_puts(&lg, RT "\n");
	log_flush(-1, &lg, true);
}
