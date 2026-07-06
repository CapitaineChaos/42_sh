/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_aux.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 00:15:57 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 20:06:27 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_exec.h"
#include "module_parser.h"
#include "module_debug.h"
#include "module_signal.h"
#include <sys/wait.h>
#include <stdio.h>

int	wait_for_children(t_ast_node **cmds, int ncmds)
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

int	get_pipeline_cmds_count(t_ast_node *node, int *count)
{
	if (!node)
		return (*count);
	if (node->type == AST_PIPE)
	{
		get_pipeline_cmds_count(node->left, count);
		get_pipeline_cmds_count(node->right, count);
	}
	else
		(*count)++;
	return (*count);
}

int	collect_pipeline_cmds(t_ast_node *node, t_ast_node **tab, int idx)
{
	if (!node)
		return (idx);
	if (node->type == AST_PIPE)
	{
		idx = collect_pipeline_cmds(node->left, tab, idx);
		idx = collect_pipeline_cmds(node->right, tab, idx);
	}
	else
	{
		tab[idx++] = node;
	}
	return (idx);
}
