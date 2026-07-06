/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:19:40 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 20:02:58 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_exec.h"
#include "module_parser.h"
#include "module_signal.h"
#include <sys/wait.h>
#include <stdio.h>

int	close_and_exit_parent(bool in, int *fd, int *prev_in)
{
	if (*prev_in != -1)
		close(*prev_in);
	if (in)
	{
		close(fd[1]);
		*prev_in = fd[0];
	}
	return (0);
}

int	pipe_and_fork(t_ast_node **cmds, bool in, int *fd, int *pid)
{
	if (in && pipe(fd) < 0)
	{
		free(cmds);
		return (1);
	}
	*pid = fork();
	if (*pid < 0)
	{
		free(cmds);
		return (1);
	}
	return (0);
}

void	close_child(int *fd, int prev_in, bool out)
{
	if (prev_in != -1)
	{
		dup2(prev_in, STDIN_FILENO);
		close(prev_in);
	}
	if (out)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
}

int	exec_single_pipe(t_ast_node **cmds, int ncmds, int i, int *prev_in)
{
	int			fd[2];
	pid_t		pid;
	t_ast_node	*node;
	int			status;

	if (pipe_and_fork(cmds, i < ncmds - 1, fd, &pid) != 0)
		return (1);
	cmds[i]->pid = pid;
	if (pid == 0)
	{
		node = cmds[i];
		sig_reset();
		close_child(fd, *prev_in, i < ncmds - 1);
		free(cmds);
		status = exec_node(node);
		free_and_exit_minishell(status);
	}
	return (close_and_exit_parent(i < ncmds - 1, fd, prev_in));
}

int	exec_pipeline(t_ast_node *root)
{
	t_ast_node	**cmds;
	int			ncmds;
	int			prev_in;
	int			i;

	ncmds = 0;
	if (get_pipeline_cmds_count(root, &ncmds) == 0)
		return (EXIT_SUCCESS);
	cmds = malloc(sizeof(*cmds) * ncmds);
	if (!cmds)
		free_and_exit_minishell(EXIT_FAILURE);
	collect_pipeline_cmds(root, cmds, 0);
	i = 0;
	prev_in = -1;
	while (i < ncmds)
	{
		if (exec_single_pipe(cmds, ncmds, i, &prev_in) != 0)
			return (EXIT_FAILURE);
		i++;
	}
	return (return_full_code(wait_for_children(cmds, ncmds)));
}
