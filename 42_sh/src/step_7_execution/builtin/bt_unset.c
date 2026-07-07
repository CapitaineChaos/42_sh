/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 04:29:52 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 20:37:17 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_builtin.h"
#include "module_minishell.h"
#include "utils.h"
#include "module_env.h"
#include "helpers.h"

int	builtin_unset(t_mns *mns, int argc, char **argv, char **envp)
{
	t_logger	logger;
	t_optres	opt;
	int			i;

	(void)mns;
	(void)envp;
	(void)argc;
	if (bt_getopt(argv, "", NULL, &opt) < 0)
	{
		log_init(&logger);
		log_puts(&logger, "🐰: unset: ");
		log_puts(&logger, opt.badarg);
		log_puts(&logger, ": invalid option\n");
		log_puts(&logger,
			"unset: usage: unset [-f] [-v] [-n] [name ...]\n");
		log_flush(STDERR_FILENO, &logger, false);
		return (2);
	}
	i = opt.operand;
	while (argv[i])
	{
		if (is_valid_id(argv[i]))
			unsetenv(argv[i]);
		i++;
	}
	return (0);
}
