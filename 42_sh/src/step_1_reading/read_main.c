/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:04:13 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 21:23:03 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include <readline/history.h>
#include "module_read.h"
#include "module_checker.h"
#include "module_token.h"
#include "module_exec.h"
#include "module_signal.h"

static void	exit_from_heredoc(t_main_data *data)
{
	if (!isatty(STDIN_FILENO))
	{
		set_code(0);
		data->total_input.free(&data->total_input);
		data->chk->proceed_loop = false;
		checker_reset(data->chk, data->lv);
		lexer_soft_reset(data->lx, data->lv);
		free_token_list(&data->lx->tokens);
		return ;
	}
	set_code(130);
	data->total_input.free(&data->total_input);
	g_signal_flag = 0;
	data->chk->proceed_loop = true;
	checker_reset(data->chk, data->lv);
	lexer_soft_reset(data->lx, data->lv);
	free_token_list(&data->lx->tokens);
}

static void	manage_pendings(t_main_data *data)
{
	if (data->lx->state != LEX_NORMAL)
	{
		if (!isatty(STDIN_FILENO))
		{
			if (data->lx->tokens.tail == NULL
				|| data->lx->tokens.tail->type == TOK_EOF
				|| data->lx->tokens.tail->type == TOK_NEWLINE)
			{
				if (data->lx->ctxs.tail)
					print_unxpected(convert_ctx_to_sym(
							data->lx->ctxs.tail->ctx_name));
				sstr_free(&data->inputs);
				data->total_input.free(&data->total_input);
				free_and_exit_minishell(2);
			}
		}
		data->total_input.append_str(&data->total_input, "\n");
		sstrs_append(&data->inputs, strdup("\n"));
		data->lx->inp.pos += 1;
	}
}

bool	main_loop(t_main_data *data)
{
	sstrs_append(&data->inputs, data->partial_input);
	data->total_input.append_str(&data->total_input, data->partial_input);
	data->lx->inp.stream = data->total_input.get(&data->total_input);
	run_lexer(data->lx, data->lv);
	if (data->lx->ctxs.count > 0)
		data->chk->proceed_loop = true;
	else
		data->chk->proceed_loop = false;
	if (special_checker(data, &data->lx->tokens) == -1)
	{
		set_code(2);
		checker_reset(data->chk, data->lv);
		save_history(data);
		data->total_input.free(&data->total_input);
		free_token_list(&data->lx->tokens);
		lexer_soft_reset(data->lx, data->lv);
		data->chk->proceed_loop = false;
		return (true);
	}
	manage_pendings(data);
	if (collect_heredocs(data->lx, *data->lc, data->lv) == false)
		return (exit_from_heredoc(data), true);
	return (false);
}
