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
	return (-1);
}

void	init_read(t_main_data *data)
{
	data->partial_input = NULL;
	if (data->stream != NULL)
		free(data->stream);
	data->stream = NULL;
	strapi_init(L_BUFFER_SIZE, &data->total_input);
	sstr_free(&data->inputs);
	lexer_hard_reset(data->lx);
	data->lx->inp.pos = 0;
	data->chk->proceed_loop = true;
}


void	set_tokens_parts_ptrs(t_token *first, char *stream)
{
	t_token		*token;

	token = first;
	while (token)
	{
		if (token->type != TOK_HEREDOC_BODY)
			token->source = stream;
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
	dbg_reset();
	dbg_read(data->inputs);
	dbg_tokens(&data->lx->tokens);
	lexer_soft_reset(data->lx);
	save_history(data);
	checker_reset(data->chk);
	return (1);
}
