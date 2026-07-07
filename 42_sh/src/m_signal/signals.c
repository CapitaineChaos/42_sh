/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 04:42:53 by smaitre           #+#    #+#             */
/*   Updated: 2026/07/06 00:00:00 by codex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_signal.h"
#include "utils.h"
#include <readline/readline.h>

volatile sig_atomic_t	g_signal_flag = 0;

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

void	sig_parent_ignore(t_sigaction *saved)
{
	t_sigaction	ign;

	ign.sa_handler = SIG_IGN;
	ign.sa_flags = 0;
	sigemptyset(&ign.sa_mask);
	sigaction(SIGINT, NULL, &saved[0]);
	sigaction(SIGQUIT, NULL, &saved[1]);
	sigaction(SIGINT, &ign, NULL);
	sigaction(SIGQUIT, &ign, NULL);
}

void	sig_parent_restore(t_sigaction *saved)
{
	sigaction(SIGINT, &saved[0], NULL);
	sigaction(SIGQUIT, &saved[1], NULL);
}

void	sig_reset(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGTSTP, &sa, NULL);
	sigaction(SIGPIPE, &sa, NULL);
}

void	first_time_init(void)
{
	struct sigaction	sa;

	rl_event_hook = sig_event;
	rl_change_environment = 0;
	g_signal_flag = 0;
	signal(SIGINT, sigint_standard);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGPIPE, &sa, NULL) < 0)
		perror("sigaction(SIGPIPE)");
}

void	init_signals(void)
{
	t_logger	logger;

	rl_event_hook = sig_event;
	if (g_signal_flag == 130 || g_signal_flag == 131)
	{
		log_init(&logger);
		if (g_signal_flag == 131)
			log_puts(&logger, "Quit (core dumped)");
		log_puts(&logger, "\n");
		log_flush(STDERR_FILENO, &logger, false);
	}
	g_signal_flag = 0;
	signal(SIGINT, sigint_standard);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}
