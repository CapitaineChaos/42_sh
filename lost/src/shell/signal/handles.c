/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 17:23:09 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 02:15:51 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_signal.h"
#include <readline/readline.h>
#include <errno.h>
#include "module_minishell.h"
#include "module_context.h"
#include "module_lexer.h"
#include "module_token.h"
#include <sys/wait.h>

void	sigint_standard(int status)
{
	(void)status;
	rl_done = 1;
	g_signal_flag = 1;
}

void	ignore_sigquit(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

void	sigint_heredoc(int status)
{
	(void)status;
	rl_replace_line("", 0);
	rl_redisplay();
	rl_done = 1;
	g_signal_flag = 1;
}

int	sig_event(void)
{
	ignore_sigquit();
	return (EXIT_SUCCESS);
}
