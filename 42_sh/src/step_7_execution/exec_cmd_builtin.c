/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_builtin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 02:00:34 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 21:36:48 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_exec.h"
#include <stdio.h>
#include <unistd.h>

static void	restore_fds(int sv_in, int sv_out)
{
	dup2(sv_in, STDIN_FILENO);
	dup2(sv_out, STDOUT_FILENO);
	close(sv_in);
	close(sv_out);
}

static int	check_app_r(t_ast_node *node, int sv_in, int sv_out)
{
	if (apply_redirs(node) != 0)
	{
		restore_fds(sv_in, sv_out);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static bool	is_special_builtin(char *cmd)
{
	if (strcmp(cmd, ".") == 0)
		return (true);
	if (strcmp(cmd, "exit") == 0)
		return (true);
	if (strcmp(cmd, "export") == 0)
		return (true);
	if (strcmp(cmd, "unset") == 0)
		return (true);
	return (false);
}

static int	return_builtin_status(char **argv, int ret)
{
	if (ret != 0 && !isatty(STDIN_FILENO) && is_special_builtin(argv[0]))
		free_and_exit_minishell(ret);
	return (ret);
}

static int	do_exit_builtin(char **argv, t_operand *op, int sv_in, int sv_out)
{
	restore_fds(sv_in, sv_out);
	return (execute_builtin(op->argc, argv));
}

int	exec_builtin(t_ast_node *node, char **argv, t_operand *op)
{
	int	sv_in;
	int	sv_out;
	int	ret;

	debug_mini_title(LVL_EXEC, "Exec builtin");
	debug_str_array("Exec builtin argv", argv);
	if (op->redirections.count == 0)
	{
		ret = execute_builtin(op->argc, argv);
		return (return_builtin_status(argv, ret));
	}
	sv_in = dup(STDIN_FILENO);
	sv_out = dup(STDOUT_FILENO);
	if (sv_in < 0 || sv_out < 0)
		return (return_code(EXIT_FAILURE, "dup"));
	if (check_app_r(node, sv_in, sv_out) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (strcmp(op->argv[0], "exit") == 0)
		return (do_exit_builtin(argv, op, sv_in, sv_out));
	ret = execute_builtin(op->argc, argv);
	restore_fds(sv_in, sv_out);
	return (return_builtin_status(argv, ret));
}
