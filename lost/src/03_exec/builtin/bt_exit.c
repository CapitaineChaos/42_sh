/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 11:41:53 by cmassol           #+#    #+#             */
/*   Updated: 2025/05/28 20:58:55 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_minishell.h"
#include "ft_std.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

static void	exit_and_free_non_numeric(char *exit_str)
{
	t_logger	lg;

	log_init(&lg);
	log_puts(&lg, "exit: ");
	log_puts(&lg, exit_str);
	log_puts(&lg, ": numeric argument required\n");
	log_flush(STDERR_FILENO, &lg, false);
	free_and_exit_minishell(2);
}

void	check_nullargs(t_mns *mns, int argc, char **argv, char **envp)
{
	t_logger	lg;

	(void)mns;
	(void)argc;
	(void)argv;
	(void)envp;
	log_init(&lg);
	if (argv[1] == NULL)
	{
		log_puts(&lg, "exit\n");
		if (isatty(STDERR_FILENO))
			log_flush(STDERR_FILENO, &lg, false);
		free_and_exit_minishell(EXIT_SUCCESS);
	}
}

int	builtin_exit(t_mns *mns, int argc, char **argv, char **envp)
{
	t_logger	lg;
	intmax_t	exit_status;
	bool		i_error;

	check_nullargs(mns, argc, argv, envp);
	log_init(&lg);
	if (strcmp(argv[1], "--") == 0)
	{
		argv++;
		argc--;
	}
	exit_status = ft_atoll_secure(argv[1], &i_error);
	if (i_error)
		exit_and_free_non_numeric(argv[1]);
	else
	{
		if (argc > 2)
		{
			log_puts(&lg, "exit: too many arguments\n");
			log_flush(STDERR_FILENO, &lg, false);
			return (1);
		}
		free_and_exit_minishell (exit_status);
	}
	return (0);
}
