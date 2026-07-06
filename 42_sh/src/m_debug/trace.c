/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 15:26:08 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 20:37:17 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_config.h"
#include "colors.h"
#include "module_debug.h"
#include "utils.h"

static bool	trace_enabled(int lvl, char *str)
{
	return (DLVL >= lvl && str != NULL);
}

static void	trace_prefix(t_logger *lg, int lvl)
{
	log_puts(lg, BBL "[");
	log_puts(lg, convert_dbg_lvl_name(lvl));
	log_puts(lg, "] ");
}

static void	trace_status(int lvl, char *label, char *color, char *icon,
	char *str)
{
	t_logger	lg;

	if (!trace_enabled(lvl, str))
		return ;
	log_init(&lg);
	trace_prefix(&lg, lvl);
	log_puts(&lg, LGR " > ");
	log_puts(&lg, color);
	log_puts(&lg, label);
	log_puts(&lg, RT LGR);
	log_puts(&lg, icon);
	log_puts(&lg, str);
	log_puts(&lg, RT "\n");
	log_flush(-1, &lg, true);
}

void	trace_ok(int lvl, char *str)
{
	trace_status(lvl, "[OK]", GN, "    📡 ", str);
}

void	trace_ko(int lvl, char *str)
{
	trace_status(lvl, "[KO]", RD, " 🛑 ", str);
}

void	trace_start(int lvl, char *str)
{
	trace_status(lvl, "[START]", YL, " 🧻 ", str);
}

void	trace_fatal(int lvl, char *str)
{
	t_logger	lg;

	if (str == NULL)
		return ;
	log_init(&lg);
	trace_prefix(&lg, lvl);
	log_puts(&lg, LGR " > " RD "@@@[ FATAL ]@@@" RT LGR "     ");
	log_puts(&lg, str);
	log_puts(&lg, RT "\n");
	log_flush(-1, &lg, true);
}

void	trace_info(int lvl, char *str)
{
	trace_status(lvl, "[INFO]", CY, "     ", str);
}

void	trace_short_info(int lv, char *str)
{
	t_logger	lg;

	if (!trace_enabled(lv, str))
		return ;
	log_init(&lg);
	trace_prefix(&lg, lv);
	log_puts(&lg, "   " LGR);
	log_puts(&lg, str);
	log_puts(&lg, RT "\n");
	log_flush(-1, &lg, true);
}

void	trace_short_info_no_nl(int lv, char *str)
{
	t_logger	lg;

	if (!trace_enabled(lv, str))
		return ;
	log_init(&lg);
	log_puts(&lg, str);
	log_flush(-1, &lg, true);
}

static void	trace_named_value(int lv, char *name, char *mark)
{
	t_logger	lg;

	log_init(&lg);
	trace_prefix(&lg, lv);
	log_puts(&lg, LGR " > " CY "[INFO]" RT);
	log_puts(&lg, mark);
	log_puts(&lg, name);
	log_puts(&lg, " : ");
	log_flush(-1, &lg, true);
}

void	trace_info_nvchr(int lv, char *name, char c)
{
	t_logger	lg;

	if (DLVL < lv || name == NULL)
		return ;
	trace_named_value(lv, name, "     ");
	log_init(&lg);
	log_putch(&lg, c);
	log_puts(&lg, RT "\n");
	log_flush(-1, &lg, true);
}

void	trace_info_nvstr(int lv, char *name, char *value)
{
	t_logger	lg;

	if (DLVL < lv || name == NULL)
		return ;
	trace_named_value(lv, name, "     ");
	log_init(&lg);
	log_puts(&lg, value ? value : "NULL");
	log_puts(&lg, RT "\n");
	log_flush(-1, &lg, true);
}

void	trace_info_nvnb(int lv, char *name, int value)
{
	t_logger	lg;

	if (DLVL < lv || name == NULL)
		return ;
	trace_named_value(lv, name, "  📌 ");
	log_init(&lg);
	log_putlong(&lg, value);
	log_puts(&lg, RT "\n");
	log_flush(-1, &lg, true);
}

void	trace_info_nvcharpos(int lv, char *name, int pos, char c)
{
	t_logger	lg;

	if (DLVL < lv || name == NULL)
		return ;
	trace_named_value(lv, name, "     ");
	log_init(&lg);
	log_puts(&lg, "[");
	log_putlong(&lg, pos);
	log_puts(&lg, "]     -   ");
	log_putch(&lg, c);
	log_puts(&lg, RT "\n");
	log_flush(-1, &lg, true);
}
