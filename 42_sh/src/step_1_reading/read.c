/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:12:27 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 00:03:19 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_signal.h"
#include "module_read.h"
#include "module_minishell.h"
#include "helpers.h"
#include "utils.h"
#include <readline/readline.h>

static char	*get_line(t_main_data *data)
{
	char	*line;
	char	*prompt;

	if (isatty(STDIN_FILENO))
	{
		prompt = get_prompt(&data->lx->ctxs);
		init_signals();
		line = readline(prompt);
		free(prompt);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		line = nano_get_next_line(STDIN_FILENO);
	}
	return (line);
}

int	read_user_input(t_main_data *data)
{
	init_read(data);
	while (data->chk->proceed_loop)
	{
		data->partial_input = get_line(data);
		*(data->lc) += 1;
		if (main_stop(data))
			return (0);
		if (main_sigint(data))
			continue ;
		if (main_loop(data))
			return (1);
	}
	return (main_finalize(data));
}
