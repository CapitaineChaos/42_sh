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

static int	check_app_r(t_ast_node *node, int sv_in, int sv_out)
{
	if (apply_redirs(node) != 0)
	{
		dup2(sv_in, STDIN_FILENO);
		dup2(sv_out, STDOUT_FILENO);
		close(sv_in);
		close(sv_out);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	do_exit_builtin(char **argv, t_operand *op, int sv_in, int sv_out)
{
	dup2(sv_in, STDIN_FILENO);
	dup2(sv_out, STDOUT_FILENO);
	close(sv_in);
	close(sv_out);
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
		return (execute_builtin(op->argc, argv));
	sv_in = dup(STDIN_FILENO);
	sv_out = dup(STDOUT_FILENO);
	if (sv_in < 0 || sv_out < 0)
		return (return_code(EXIT_FAILURE, "dup"));
	if (check_app_r(node, sv_in, sv_out) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (strcmp(op->argv[0], "exit") == 0)
		return (do_exit_builtin(argv, op, sv_in, sv_out));
	ret = execute_builtin(op->argc, argv);
	dup2(sv_in, STDIN_FILENO);
	dup2(sv_out, STDOUT_FILENO);
	close(sv_in);
	close(sv_out);
	return (ret);
}
