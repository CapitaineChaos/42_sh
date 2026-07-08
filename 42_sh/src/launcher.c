/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launcher.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:17:42 by cmassol           #+#    #+#             */
/*   Updated: 2025/05/29 21:35:39 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_signal.h"
#include "texts.h"
#include "module_minishell.h"
#include <stdlib.h>
#include <string.h>

void	launch_animation(t_mns *mns)
{
	int		cols;
	int		rows;
	char	*lvl;

	if (mns->posix)
		return ;
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
		return ;
	lvl = getenv("SHLVL");
	if (lvl && atoi(lvl) > 1)
		return ;
	get_terminal_size(&cols, &rows);
	if (cols >= 122)
		select_animation("gits_lines");
	else if (cols >= 80)
		select_animation("gits_lines_middle");
	else if (cols >= 49)
		select_animation("gits_lines_mini");
	else if (cols <= 48)
		select_animation("gits_lines_micro");
}

/**
 * @brief Point d'entrée du programme.
 */
int	main(int ac, char **av, char **envp)
{
	t_mns	mns;
	int		exit_code;
	bool	posix;

	posix = (ac == 2 && !strcmp(av[1], "--posix"));
	if (ac > 1 && !posix)
	{
		write(STDERR_FILENO, "Usage: ./minishell [--posix]\n", 29);
		return (1);
	}
	dbg_errors_reset();
	init_mns(&mns, strdup(" \t\n"), envp);
	mns.posix = posix;
	first_time_init();
	mns.argv = av;
	mns.argc = ac;
	launch_animation(&mns);
	minishell(&mns);
	exit_code = mns.last_exit_code;
	free_mns(&mns);
	if (!posix && isatty(STDERR_FILENO))
		write(STDERR_FILENO, "End of MiNIShell 🐰🐯\n", 26);
	return (exit_code);
}
