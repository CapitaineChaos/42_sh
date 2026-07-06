/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 18:59:57 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 20:37:17 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_read.h"
#include "module_lexer.h"
#include "module_checker.h" 
#include <readline/history.h>

void	save_history(t_main_data *data)
{
	char	*flat;

	if (!isatty(STDIN_FILENO))
		return ;
	flat = sstrs_flatten(data->inputs);
	trace_info_nvstr(LVL_MNS, "History - Full user input", flat);
	add_history(flat);
	free(flat);
}

int	print_unxpected(char *s)
{
	t_logger	lg;

	log_init(&lg);
	log_puts(&lg, "🐰: unexpected EOF while looking for matching `");
	log_puts(&lg, s);
	log_puts(&lg, "'\n");
	log_flush(STDERR_FILENO, &lg, false);
	trace_logger_flush(-1, &lg, true);
	return (-1);
}

void	init_read(t_main_data *data)
{
	trace_info(LVL_INPUT, "Start user input");
	data->partial_input = NULL;
	if (data->stream != NULL)
		free(data->stream);
	data->stream = NULL;
	strapi_init(L_BUFFER_SIZE, &data->total_input);
	sstr_free(&data->inputs);
	lexer_hard_reset(data->lx, data->lv);
	data->lx->inp.pos = 0;
	data->chk->proceed_loop = true;
}


void	set_tokens_parts_ptrs(t_token *first, char *stream)
{
	t_token		*token;
	t_tk_part	*part;

	token = first;
	while (token)
	{
		part = token->head;
		while (part)
		{
			if (token->type == TOK_HEREDOC_BODY)
			{
				part->stream = part->str;
				part = part->next;
				continue ;
			}
			part->stream = stream + part->offset;
			part = part->next;
		}
		token = token->next;
	}
}

/**
 * @brief 	Finalise la lecture de l'input utilisateur.
 * @param 	data	Les données de lecture.
 * @note    Clôture du mode multiligne.
 */
int	main_finalize(t_main_data *data)
{
	data->stream = data->total_input.get(&data->total_input);
	set_tokens_parts_ptrs(data->lx->tokens.head, data->stream);
	debug_tokens(&data->lx->tokens, data->lv);
	trace_info(LVL_INPUT, "  Fin user input  ");
	lexer_soft_reset(data->lx, data->lv);
	trace_info_nvstr(LVL_MNS, "Stream", data->stream);
	save_history(data);
	trace_info_nvstr(LVL_INPUT, "Stream", data->stream);
	checker_reset(data->chk, data->lv);
	return (1);
}
