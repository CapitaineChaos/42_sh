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
#include "module_env.h"
#include "all_config.h"
#include <stdlib.h>
#include <readline/readline.h>

void	launch_animation(t_mns *mns)
{
	int		cols;
	int		rows;
	char	*lvl;

	(void)mns;
	if (DLVL > -2)
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

void	interactive_shell_only(void)
{
	if (DLVL < -1 && (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)))
	{
		write(STDERR_FILENO,
			"minishell: non-interactive mode is not allowed\n", 47);
		exit(1);
	}
}

/**
 * @brief Point d'entrée du programme.
 * 
 * @note 1 : input
 * @note 2 : checker
 * @note 3 : lexer
 * @note 4 : parser
 * @note 5 : expansion
 * @note 6 : assembling
 * @note 7 : execution
 */
int	main(int ac, char **av, char **envp)
{
	t_mns	mns;
	int		exit_code;
	int		lv;

	if (ac > 1)
	{
		write(STDERR_FILENO, "Usage: ./minishell\n", 20);
		return (1);
	}
	interactive_shell_only();
	lv = 7;
	dbg_errors_reset();
	init_mns(&mns, strdup(" \t\n"), envp, lv);
	first_time_init();
	mns.argv = av;
	mns.argc = ac;
	launch_animation(&mns);
	minishell(&mns, lv);
	exit_code = mns.last_exit_code;
	free_mns(&mns);
	if (isatty(STDERR_FILENO))
		write(STDERR_FILENO, "End of MiNIShell 🐰🐯\n", 26);
	return (exit_code);
}
