/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:19:40 by smaitre           #+#    #+#             */
/*   Updated: 2026/07/06 00:00:00 by codex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_exec.h"
#include "module_parser.h"
#include "module_signal.h"
#include <sys/wait.h>

static int	wait_for_children(t_ast_node **cmds, int ncmds)
{
	int	i;
	int	status;

	i = 0;
	while (i < ncmds)
	{
		status = 0;
		if (cmds[i]->pid > 0)
			waitpid(cmds[i]->pid, &status, 0);
		cmds[i]->status = status;
		i++;
	}
	status = cmds[ncmds - 1]->status;
	free(cmds);
	return (status);
}

static int	count_pipeline_cmds(t_ast_node *node)
{
	if (!node)
		return (0);
	if (node->type == AST_PIPE)
		return (count_pipeline_cmds(node->left)
			+ count_pipeline_cmds(node->right));
	return (1);
}

static int	collect_pipeline_cmds(t_ast_node *node, t_ast_node **tab, int idx)
{
	if (!node)
		return (idx);
	if (node->type == AST_PIPE)
	{
		idx = collect_pipeline_cmds(node->left, tab, idx);
		idx = collect_pipeline_cmds(node->right, tab, idx);
	}
	else
		tab[idx++] = node;
	return (idx);
}

static int	close_and_exit_parent(bool has_pipe, int *fd, int *prev_in)
{
	if (*prev_in != -1)
		close(*prev_in);
	if (has_pipe)
	{
		close(fd[1]);
		*prev_in = fd[0];
	}
	return (0);
}

static int	pipe_and_fork(t_ast_node **cmds, bool has_pipe, int *fd, int *pid)
{
	if (has_pipe && pipe(fd) < 0)
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

static void	close_child(int *fd, int prev_in, bool has_output)
{
	if (prev_in != -1)
	{
		dup2(prev_in, STDIN_FILENO);
		close(prev_in);
	}
	if (has_output)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
}

static int	exec_single_pipe(t_ast_node **cmds, int ncmds, int i, int *prev_in)
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

	ncmds = count_pipeline_cmds(root);
	if (ncmds == 0)
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
