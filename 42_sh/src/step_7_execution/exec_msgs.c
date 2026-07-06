/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_msgs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 02:18:22 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 20:00:20 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "utils.h"
#include "module_exec.h"
#include <errno.h>
#include <unistd.h>
#include <signal.h>

int	disp_msg_and_ret(char *msg, int ret)
{
	t_logger	lg;

	log_init(&lg);
	log_puts(&lg, "🐰🐯 Minishell: ");
	log_puts(&lg, msg);
	log_flush(STDERR_FILENO, &lg, false);
	trace_logger_flush(-1, &lg, true);
	return (ret);
}

int	display_other_msg(int sig, bool be_pipe)
{
	t_logger	lg;

	if (sig != SIGPIPE + 128)
		return (sig);
	log_init(&lg);
	log_puts(&lg, "🐰🐯 Minishell: ");
	log_puts(&lg, "Broken pipe\n");
	if (be_pipe)
		log_flush(STDERR_FILENO, &lg, false);
	trace_logger_flush(-1, &lg, true);
	return (sig);
}

void	display_msg_err(char *cmd, int err)
{
	t_logger	lg;

	log_init(&lg);
	log_puts(&lg, "🐯🐰 Minishell: ");
	log_puts(&lg, cmd);
	if (err == EACCES)
		log_puts(&lg, ": Permission denied\n");
	else if (err == EISDIR)
		log_puts(&lg, ": Is a directory\n");
	else if (err == ENOENT)
		log_puts(&lg, ": No such file or directory\n");
	else
		log_puts(&lg, ": Unknown error\n");
	log_flush(STDERR_FILENO, &lg, false);
	trace_logger_flush(-1, &lg, true);
}

void	display_cmd_err(char *cmd)
{
	t_logger	lg;

	log_init(&lg);
	log_puts(&lg, "🐰🐯 Minishell: ");
	log_puts(&lg, cmd);
	log_puts(&lg, ": command not found\n");
	log_flush(STDERR_FILENO, &lg, false);
	trace_logger_flush(-1, &lg, true);
}
