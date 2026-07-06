/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_debug_lvl.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 19:46:35 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 20:37:17 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_config.h"
#include "module_debug.h"
#include "colors.h"
#include "utils.h"

static void	get_level(t_logger *lg)
{
	if (DLVL > 0)
		log_puts(lg, "=============== 🐰x 🐯 ================\n");
	if (DLVL >= LVL_CHK)
		log_puts(lg, CY "    CHECKER " RT "debug level\n");
	if (DLVL >= LVL_LEXER)
		log_puts(lg, CY "    LEXER " RT "debug level\n");
	if (DLVL >= LVL_HEREDOC)
		log_puts(lg, CY "    HEREDOC " RT "debug level\n");
	if (DLVL >= LVL_PARSER)
		log_puts(lg, CY "    PARSER " RT "debug level\n");
	if (DLVL >= LVL_AST)
		log_puts(lg, CY "    AST " RT "debug level\n");
	if (DLVL >= LVL_EXPAND)
		log_puts(lg, CY "    EXPAND " RT "debug level\n");
	if (DLVL >= LVL_ARGV)
		log_puts(lg, CY "    ARGV " RT "debug level\n");
	if (DLVL >= LVL_EXEC)
		log_puts(lg, CY "    EXEC " RT "debug level\n");
}

static void	trace_terminal_size(t_logger *lg)
{
	int	cols;
	int	rows;

	get_terminal_size(&cols, &rows);
	log_puts(lg, "Terminal size: ");
	log_putlong(lg, cols);
	log_puts(lg, " columns, ");
	log_putlong(lg, rows);
	log_puts(lg, " rows\n\n");
}

void	trace_debug_level(void)
{
	t_logger	lg;

	if (!(DLVL > -1))
		return ;
	log_init(&lg);
	log_puts(&lg, "\nLVL ");
	log_putlong(&lg, DLVL);
	log_puts(&lg, "\n\n");
	get_level(&lg);
	log_puts(&lg, "=======================================\n");
	log_puts(&lg, GN  "\n  Start of MiNIShell 🐰🐯\n");
	log_puts(&lg, "   C'est un nouveau jour qui s'eveille !\n");
	log_puts(&lg, "   Une nouvelle ère commence !\n");
	log_puts(&lg, "   Le Lapinocène suivi du Wildcatocène.\n");
	log_puts(&lg, "   Ce monde est désormais le Nôtre 🌴🌴🌴🌴!\n\n" RT);
	if (DLVL >= LVL_MNS)
	{
		trace_terminal_size(&lg);
	}
	log_flush(-1, &lg, true);
}
