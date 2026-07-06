/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_external.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 02:04:58 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 00:03:02 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_exec.h"
#include "module_signal.h"
#include <errno.h>
#include <sys/wait.h>

int	exec_external(t_ast_node *node, char *path, char **av, char **ep)
{
	pid_t				pid;
	int					status;
	struct sigaction	saved[2];
	int					saved_errno;

	debug_mini_title(LVL_EXEC, "Exec external");
	debug_str_array("Exec external argv", av);
	trace_info_nvstr(LVL_EXEC, "Exec external", path);
	pid = fork();
	if (pid < 0)
		return (return_code(EXIT_FAILURE, "fork"));
	if (pid == 0)
	{
		trace_info_nvstr(LVL_EXEC, "Exec redirs Child", path);
		sig_reset();
		if (apply_redirs(node) != 0)
			free_and_exit_minishell(EXIT_FAILURE);
		trace_info_nvstr(LVL_EXEC, "Execve Child", path);
		execve(path, av, ep);
		saved_errno = errno;
		trace_info_nvstr(LVL_EXEC, "External execve failed", path);
		free_and_exit_minishell(get_exit_code(av[0], saved_errno));
	}
	trace_info_nvstr(LVL_EXEC, "Execve Parent", path);
	sig_parent_ignore(saved);
	waitpid(pid, &status, 0);
	sig_parent_restore(saved);
	return (return_full_code(status));
}
