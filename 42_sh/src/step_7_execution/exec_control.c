/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_control.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:34:27 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 20:10:07 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_exec.h"
#include "module_parser.h"
#include <sys/wait.h>

int	exec_sequence(t_ast_node *node)
{
	int	status;

	status = EXIT_SUCCESS;
	if (node->left)
	{
		status = exec_node(node->left);
		set_code(get_exit_status(status));
	}
	if (node->right)
	{
		status = exec_node(node->right);
		set_code(get_exit_status(status));
	}
	return (status);
}

int	exec_and_or(t_ast_node *node)
{
	int	status;

	status = exec_node(node->left);
	if ((node->type == AST_AND && status == 0)
		|| (node->type == AST_OR && status != 0))
		return (exec_node(node->right));
	return (status);
}

int	exec_subshell(t_ast_node *node)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		return (EXIT_FAILURE);
	if (pid == 0)
	{
		if (apply_redirs(node) != 0)
			free_and_exit_minishell(EXIT_FAILURE);
		status = exec_node(node->left);
		free_and_exit_minishell(get_exit_status(status));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE);
}
