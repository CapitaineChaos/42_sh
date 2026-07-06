/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_aux.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 23:15:06 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 00:53:54 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_checker.h"
#include "module_token.h"
#include "module_exec.h"

void	disp_nomultiline(t_main_data *data, t_token *error)
{
	t_logger	lg;
	t_ctx_		ctx;

	(void)data;
	(void)error;
	if (data->lx->ctxs.tail)
	{
		log_init(&lg);
		ctx = data->lx->ctxs.tail->ctx_name;
		if (ctx == CTX___DQUOTE || ctx == CTX___SQUOTE)
			log_puts(&lg, "🐯: Syntax error: missing quote\n");
		else if (ctx == CTX___CMDAND || ctx == CTX___CMDOR || ctx == CTX___PIPE)
			log_puts(&lg, "🐯: Syntax error: missing command\n");
		else if (ctx == CTX___SUBSH)
			log_puts(&lg, "🐯: Syntax error: missing closing parenthesis\n");
		else if (ctx == CTX___ESCAPE)
			log_puts(&lg, "🐯: Syntax error: concatenat not possible\n");
	}
	else
		log_puts(&lg, "🐯: Syntax error: unexpected end of input\n");
	log_flush(STDERR_FILENO, &lg, false);
}

void	disp_stronly(t_main_data *data, t_token *error)
{
	char	*str;
	char	*line;

	line = data->lx->inp.stream;
	str = aggregate_wordparts_to_strline(error->head, line);
	print_syntax_error(str, data->partial_input);
	free(str);
}

bool	check_parens_count(t_checker *chk, int add)
{
	if (add > 0)
	{
		if (chk->parentheses < 0)
			return (true);
		chk->parentheses += add;
	}
	else if (add < 0)
	{
		chk->parentheses += add;
		if (chk->parentheses < 0)
			return (true);
	}
	return (false);
}

void	*set_states(t_checker *chk, int rdy, int cls, int cmd)
{
	if (rdy > 0)
		chk->cmd_ready = true;
	else if (rdy == 0)
		chk->cmd_ready = false;
	if (cls > 0)
		chk->cmd_closed = true;
	else if (cls == 0)
		chk->cmd_closed = false;
	if (cmd > 0)
		chk->can_cmd = true;
	else if (cmd == 0)
		chk->can_cmd = false;
	return (NULL);
}
