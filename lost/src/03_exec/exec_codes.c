/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_codes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 21:36:42 by cmassol           #+#    #+#             */
/*   Updated: 2025/05/29 20:00:59 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_exec.h"
#include "texts.h"
#include "module_signal.h"
#include "module_minishell.h"
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>

int	get_exit_status(intmax_t exit_status)
{
	if (exit_status < 0)
		exit_status = 256 + exit_status;
	else if (exit_status > 255)
		exit_status = exit_status % 256;
	return (exit_status);
}

int	return_full_code(int status)
{
	int	sig;

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		return (128 + sig);
	}
	return (EXIT_FAILURE);
}

int	return_code(int ret, char *str)
{
	perror(str);
	return (ret);
}

int	set_code(int exit_code)
{
	t_mns	*mns;

	if (exit_code == 130 || exit_code == 131)
		g_signal_flag = exit_code;
	mns = g_shell;
	mns->last_exit_code = exit_code;
	return (exit_code);
}

int	get_exit_code(char *cmd, int err)
{
	display_msg_err(cmd, err);
	if (err == EACCES || err == EISDIR)
		return (126);
	else if (err == ENOENT)
		return (127);
	else
		return (1);
}
