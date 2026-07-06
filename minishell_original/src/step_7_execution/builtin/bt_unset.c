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

	(void)envp;
	(void)argc;
	log_init(&logger);
	if (argv[1] == NULL)
		return (0);
	if (ft_strcmp(argv[1], "--") == 0)
		return (0);
	if (argv[1][0] == '-' && argv[1][1] != '\0')
	{
		log_puts(&logger, "🐰: unset: -");
		log_putch(&logger, argv[1][1]);
		log_puts(&logger, ": invalid option\n");
		log_puts(&logger,
			"unset: usage: unset [-f] [-v] [-n] [name ...]\n");
		log_flush(STDERR_FILENO, &logger, false);
		return (2);
	}
	if (!is_valid_id(argv[1]))
		return (0);
	env_list_unset(&mns->env, argv[1]);
	return (0);
}
