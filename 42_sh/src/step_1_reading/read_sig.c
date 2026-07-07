/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_sig.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:04:13 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 21:22:06 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include <readline/history.h>
#include "module_read.h"
#include "module_checker.h"
#include "module_token.h"
#include "module_exec.h"
#include "module_signal.h"

int	main_sigint(t_main_data *data)
{
	if (g_signal_flag == 1)
	{
		if (!isatty(STDIN_FILENO))
		{
			free(data->partial_input);
			sstr_free(&data->inputs);
			data->total_input.free(&data->total_input);
			free_and_exit_minishell(get_mns(NULL)->last_exit_code);
		}
		set_code(130);
		data->total_input.clear(&data->total_input);
		g_signal_flag = 0;
		free(data->partial_input);
		sstr_free(&data->inputs);
		checker_reset(data->chk, data->lv);
		lexer_soft_reset(data->lx, data->lv);
		free_token_list(&data->lx->tokens);
		return (true);
	}
	return (false);
}

bool	main_stop(t_main_data *data)
{
	if (data->partial_input == NULL)
	{
		if (!isatty(STDIN_FILENO))
		{
			sstr_free(&data->inputs);
			data->total_input.free(&data->total_input);
			free_and_exit_minishell(get_mns(NULL)->last_exit_code);
		}
		if (data->lx->ctxs.count > 0)
		{
			print_unxpected(convert_ctx_to_sym(data->lx->ctxs.tail->ctx_name));
		}
		free(data->partial_input);
		sstr_free(&data->inputs);
		data->total_input.free(&data->total_input);
		return (true);
	}
	return (false);
}
