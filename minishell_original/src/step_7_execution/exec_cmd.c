/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:19:58 by smaitre           #+#    #+#             */
/*   Updated: 2025/06/01 01:38:36 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_signal.h"
#include "module_exec.h"
#include "module_minishell.h"
#include "module_expand.h"
#include "module_builtin.h"
#include "module_parser.h"
#include "module_env.h"
#include "module_assembly.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>

/**
 * @brief Check if the command contains a slash and execute it if it does.
 */
static int	exec_slash(t_ast_node *node, t_operand *op, struct stat *sb)
{
	if (op->argv[0] && has_slash(op->argv[0]))
	{
		trace_info(LVL_EXEC, "Exec command with slash");
		if (stat(op->argv[0], sb) < 0)
		{
			display_msg_err(op->argv[0], ENOENT);
			return (127);
		}
		if (S_ISDIR(sb->st_mode))
		{
			display_msg_err(op->argv[0], EISDIR);
			return (126);
		}
		if (access(op->argv[0], X_OK) < 0)
		{
			display_msg_err(op->argv[0], EACCES);
			return (126);
		}
		return (exec_external(node, op->argv[0], op->argv, op->envp));
	}
	return (-1);
}

static int	exec_norm(t_ast_node *n, struct stat *sb, t_operand *op, t_mns *mns)
{
	char	*path_env;
	bool	path_unset;

	trace_info(LVL_EXEC, "Exec command without slash");
	op->path = get_exec_path(mns, op->argv);
	trace_info_nvstr(LVL_EXEC, "Exec path", op->path);
	if (op->path[0] == '\0')
	{
		path_env = ft_getenv(&mns->env, "PATH");
		trace_info_nvstr(LVL_EXEC, "Path env", path_env);
		path_unset = (path_env == NULL || path_env[0] == '\0');
		trace_info_nvstr(LVL_EXEC, "Path unset", path_unset ? "true" : "false");
		trace_info_nvstr(LVL_EXEC, "Command argv[0]", op->argv[0]);
		if (op->path[0] == '\0')
		{
			display_cmd_err(op->argv[0]);
			return (127);
		}
	}
	if (stat(op->path, sb) < 0)
		return (display_msg_err(op->argv[0], ENOENT), 127);
	if (S_ISDIR(sb->st_mode))
		return (display_cmd_err(op->argv[0]), 127);
	if (access(op->path, X_OK) < 0)
		return (display_msg_err(op->argv[0], EACCES), 126);
	return (exec_external(n, op->path, op->argv, op->envp));
}

static int	check_redirs_in_child(t_ast_node *node, t_operand *op)
{
	pid_t	pid;
	int		status;

	if (op->redirections.count == 0)
		return (EXIT_SUCCESS);
	trace_info(LVL_EXEC, "Check redirections in child process");
	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
	{
		if (apply_redirs(node) != EXIT_SUCCESS)
			free_and_exit_minishell(1);
		free_and_exit_minishell(0);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	exec_command(t_ast_node *node)
{
	t_operand	*op;
	t_mns		*mns;
	struct stat	sb;
	int			ret;
	
	if (node == NULL)
		return (return_code(EXIT_FAILURE, "exec command"));
	debug_mini_title(LVL_EXEC, "Exec command");
	op = &node->t_ast_data.operand;
	mns = get_mns(NULL);
	expand_args(node);
	op->envp = env_list_to_envp(&mns->env);
	ret = check_redirs_in_child(node, op);
	if (ret != 0)
	{
		debug_mini_title(LVL_EXEC, "Exec command failed");
		return (ret);
	}
	if (op->argc == 0)
		return (exec_redirects_only(node));
	if (op->argv[0] && is_builtin(op->argv[0]))
		return (exec_builtin(node, op->argv, op));
	ret = exec_slash(node, op, &sb);
	if (ret != -1)
		return (ret);
	return (exec_norm(node, &sb, op, mns));
}
