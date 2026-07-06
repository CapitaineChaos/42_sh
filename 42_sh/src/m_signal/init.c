/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 04:42:53 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 02:16:26 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_signal.h"
#include <readline/readline.h>
#include "module_context.h"
#include "module_lexer.h"
#include "module_token.h"

volatile sig_atomic_t	g_signal_flag = 0;

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

/*
**	Réinitialisation complète dans chaque fork avant exec/builtin
*/
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

/*
**	Configuration du shell interactif (processus père)
*/
void	first_time_init(void)
{
	struct sigaction	sa;

	rl_event_hook = sig_event;
	g_signal_flag = 0;
	signal(SIGINT, sigint_standard);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	ft_bzero(&sa, sizeof(sa));
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
		trace_logger_flush(-1, &logger, true);
	}
	g_signal_flag = 0;
	signal(SIGINT, sigint_standard);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	trace_info(LVL_MNS, "Signal handler");
}
