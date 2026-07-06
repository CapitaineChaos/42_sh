/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 15:26:08 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 20:37:17 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_config.h"
#include "module_debug.h"
#include "utils.h"
#include "utils.h"
#include "colors.h"

void	trace_ok(int lvl, char *str)
{
	t_logger	lg;

	if (DLVL < lvl || str == NULL)
		return ;
	log_init(&lg);
	log_puts(&lg, BBL "[");
	log_puts(&lg, convert_dbg_lvl_name(lvl));
	log_puts(&lg, "] " LGR);
	log_puts(&lg, LGR " > " GN "[OK]" RT LGR "    📡 ");
	log_puts(&lg, str);
	log_puts(&lg, RT "\n");
	log_flush(-1, &lg, true);
}

void	trace_ko(int lvl, char *str)
{
	t_logger	lg;

	if (DLVL < lvl || str == NULL)
		return ;
	log_init(&lg);
	log_puts(&lg, BBL "[");
	log_puts(&lg, convert_dbg_lvl_name(lvl));
	log_puts(&lg, "] " LGR);
	log_puts(&lg, LGR " > " RD "[KO]" RT LGR " 🛑 ");
	log_puts(&lg, str);
	log_puts(&lg, RT "\n");
	log_flush(-1, &lg, true);
}

void	trace_start(int lvl, char *str)
{
	t_logger	lg;

	if (DLVL < lvl || str == NULL)
		return ;
	log_init(&lg);
	log_puts(&lg, BBL "[");
	log_puts(&lg, convert_dbg_lvl_name(lvl));
	log_puts(&lg, "] " LGR);
	log_puts(&lg, LGR " > " YL "[START]" RT LGR " 🧻 ");
	log_puts(&lg, str);
	log_puts(&lg, RT "\n");
	log_flush(-1, &lg, true);
}

void	trace_fatal(int lvl, char *str)
{
	t_logger	lg;

	if (str == NULL)
		return ;
	log_init(&lg);
	log_puts(&lg, BBL "[");
	log_puts(&lg, convert_dbg_lvl_name(lvl));
	log_puts(&lg, "] " LGR);
	log_puts(&lg, LGR " > " RD "@@@[ FATAL ]@@@" RT LGR "     ");
	log_puts(&lg, str);
	log_puts(&lg, RT "\n");
	log_flush(-1, &lg, true);
}

void	trace_info(int lvl, char *str)
{
	t_logger	lg;

	if (DLVL < lvl || str == NULL)
		return ;
	log_init(&lg);
	log_puts(&lg, BBL "[");
	log_puts(&lg, convert_dbg_lvl_name(lvl));
	log_puts(&lg, "] " LGR);
	log_puts(&lg, LGR " > " CY "[INFO]" RT LGR "     ");
	log_puts(&lg, str);
	log_puts(&lg, RT "\n");
	log_flush(-1, &lg, true);
}
