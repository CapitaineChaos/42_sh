/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_arg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 14:18:17 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 20:37:17 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>

#define MAX_DISPLAY   70
#define TRUNCATE_LEN  67
#define BUF_SIZE     128


void	print_name(char *name, t_logger *lg)
{
	log_puts(lg, "\n============= Printing ");
	log_puts(lg, name);
	log_puts(lg, " ============\n Array :\n");
}


void debug_argv(char *name, char **argv)
{
    int   i;
    char  buf[BUF_SIZE];
    char  tmp[MAX_DISPLAY + 1];
    size_t len;

    if (!argv)
    {
        trace_info_nvstr(LVL_ARGV, name, "[NULL]");
        return;
    }
    debug_pico_title(LVL_ARGV, "Printing argv", "\001\033[95m\002");
    for (i = 0; argv[i]; i++)
    {
        len = strlen(argv[i]);
        if (len > MAX_DISPLAY)
        {
            /* tronquer à TRUNCATE_LEN et ajouter "..." */
            memcpy(tmp, argv[i], TRUNCATE_LEN);
            tmp[TRUNCATE_LEN    ] = '.';
            tmp[TRUNCATE_LEN + 1] = '.';
            tmp[TRUNCATE_LEN + 2] = '.';
            tmp[TRUNCATE_LEN + 3] = '\0';
            snprintf(buf, sizeof(buf),
                     "    [%d] = \"%s\"\n", i, tmp);
        }
        else
        {
            snprintf(buf, sizeof(buf),
                     "    [%d] = \"%s\"\n", i, argv[i]);
        }
        debug_log(buf);
    }
    debug_pico_title(LVL_ARGV, "End of argv", "\001\033[95m\002");
}


void	debug_str_array(char *name, char **argv)
{
	int			i;
	t_logger	lg;

	if (DLVL < LVL_ARGV)
		return ;
	log_init(&lg);
	if (!argv)
	{
		log_puts(&lg, "    -> Array is NULL\n");
		log_flush(-1, &lg, true);
		return ;
	}
	i = 0;
	print_name(name, &lg);
	while (argv[i])
	{
		log_puts(&lg, "    [");
		log_putlong(&lg, i);
		log_puts(&lg, "] = ");
		log_puts(&lg, argv[i]);
		log_putch(&lg, '\n');
		i++;
	}
	log_puts(&lg, "============== End of ARRAY =============\n\n");
	log_flush(-1, &lg, true);
}
