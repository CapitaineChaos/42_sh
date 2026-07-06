/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_signal.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 02:18:14 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 02:15:57 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODULE_SIGNAL_H
# define MODULE_SIGNAL_H

# ifndef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE 200809L
# endif

# define SIGCHLD_HANDLER_FLAGS  SA_RESTART

# include <signal.h>
# include <sys/types.h>

extern volatile sig_atomic_t	g_signal_flag;
typedef struct sigaction		t_sigaction;

/**
 * INIT
 */

void	sig_parent_ignore(t_sigaction *saved);
void	sig_parent_restore(t_sigaction *saved);
void	sig_reset(void);
void	init_signals(void);
void	first_time_init(void);

/**
 * HANDLERS
 */

void	sigint_standard(int status);
void	ignore_sigquit(void);
void	sigint_heredoc(int status);
int		sig_event(void);

#endif