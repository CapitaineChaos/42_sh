/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 22:31:48 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 01:16:34 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_checker.h"
#include "module_token.h"
#include "module_exec.h"
#include "ft_std.h"

void	checker_init(t_checker *checker, int lv)
{
	if (lv < 2)
		return ;
	trace_info(LVL_CHK, "Checker init");
	memset(checker, 0, sizeof(t_checker));
	checker->proceed_loop = true;
}

void	checker_reset(t_checker *checker, int lv)
{
	if (lv < 2)
		return ;
	checker->parentheses = 0;
	checker->inp.stream = NULL;
	checker->inp.pos = 0;
	checker->current = NULL;
	checker->can_cmd = false;
	checker->cmd_ready = false;
	checker->pending_escape = false;
	checker->proceed_loop = true;
}

static void	display_unclosed_context(t_main_data *data, t_token *error)
{
	t_logger	lg;
	t_ctx_		ctx;

	(void)error;
	log_init(&lg);
	if (data->lx->ctxs.tail)
	{
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

static void	display_wordpart_error(t_main_data *data, t_token *error)
{
	char	*str;
	char	*line;

	line = data->lx->inp.stream;
	str = aggregate_wordparts_to_strline(error->head, line);
	print_syntax_error(str, data->partial_input);
	free(str);
}

static void	*set_states(t_checker *chk, int rdy, int cls, int cmd)
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

static bool	is_bad_redir(t_token **token)
{
	if (*token && (*token)->family == TKF_REDIRECT)
	{
		*token = (*token)->next;
		if (!(*token) || (*token)->type == TOK_WORD)
			return (false);
		else
		{
			trace_info(LVL_CHK, "Bad redirection");
			return (true);
		}
	}
	return (false);
}

static bool	is_bad_cmd(t_token **token)
{
	while (*token && (*token)->kind == TKD_OPERAND)
	{
		if (is_bad_redir(token))
			return (true);
		if (*token && (*token)->next && (*token)->next->kind == TKD_OPERAND)
			*token = (*token)->next;
		else
			return (false);
	}
	return (false);
}

static bool	check_lparen(t_checker *chk, t_token **cur, t_token **ret)
{
	*ret = NULL;
	if (!*cur || (*cur)->type != TOK_LPAREN)
		return (false);
	if (chk->cmd_ready && !chk->cmd_closed)
	{
		if ((*cur)->prev && (*cur)->prev->kind == TKD_OPERAND)
		{
			*ret = (*cur)->next;
			if (!(*cur)->prev->prev)
				return (true);
			if ((*cur)->prev->prev->kind != TKD_OPERAND)
				return (true);
		}
		*ret = *cur;
		return (true);
	}
	set_states(chk, 0, 0, 1);
	chk->parentheses += 1;
	return (true);
}

static bool	check_parentheses(t_checker *chk, t_token **cur, t_token **ret)
{
	*ret = NULL;
	if (check_lparen(chk, cur, ret))
		return (true);
	if (*cur && (*cur)->type == TOK_RPAREN)
	{
		if (!chk->cmd_ready)
		{
			*ret = *cur;
			return (true);
		}
		chk->parentheses -= 1;
		if (chk->parentheses < 0)
		{
			*ret = *cur;
			return (true);
		}
		*ret = set_states(chk, 1, 0, 0);
		return (true);
	}
	return (false);
}

static bool	check_command(t_checker *chk, t_token **cur, t_token **ret)
{
	if (*cur && (*cur)->family == TKF_WORD)
	{
		*ret = *cur;
		if (!chk->can_cmd)
			return (true);
		if (is_bad_cmd(cur))
		{
			*ret = *cur;
			return (true);
		}
		*ret = set_states(chk, 1, 0, 0);
		return (true);
	}
	if (*cur && (*cur)->family == TKF_REDIRECT)
	{
		if (is_bad_redir(cur))
		{
			*ret = *cur;
			return (true);
		}
		set_states(chk, 1, 0, -1);
		return (true);
	}
	return (false);
}

static bool	check_separator_or_unknown(t_checker *chk, t_token **cur, t_token **ret)
{
	if (*cur && (*cur)->family == TKF_CONTROL)
	{
		*ret = *cur;
		if (!chk->cmd_ready)
			return (true);
		set_states(chk, 1, 1, 1);
		*ret = NULL;
		return (true);
	}
	if (*cur && (*cur)->family == TKF_OPERATOR)
	{
		*ret = *cur;
		if (!chk->cmd_ready)
			return (true);
		set_states(chk, 0, 0, 1);
		*ret = NULL;
		return (true);
	}
	*ret = *cur;
	return (true);
}

static t_token	*check_token(t_checker *chk, t_token **cur)
{
	t_token	*ret;

	ret = NULL;
	trace_info_nvstr(LVL_CHK, "Current token", debug_get_token_type((*cur)->type));
	if ((*cur)->type == TOK_NEWLINE)
		set_states(chk, 0, 1, 1);
	else if (check_parentheses(chk, cur, &ret))
		return (ret);
	else if (check_command(chk, cur, &ret))
		return (ret);
	else if (check_separator_or_unknown(chk, cur, &ret))
		return (ret);
	return (ret);
}

static t_token	*init_checker(t_checker *chk, t_tokens *tokens)
{
	t_token		*cur;
	
	if (chk->current != NULL)
	{
		debug_mini_title(LVL_CHK, "Reprise du checking");
		cur = chk->current;
	}
	else
	{
		debug_title(LVL_CHK, "[  Special checker  ]");
		chk->cmd_ready = false;
		chk->cmd_closed = true;
		chk->can_cmd = true;
		chk->parentheses = 0;
		cur = tokens->head;
	}
	return (cur);
}

static t_token	*check_tokens(t_main_data *data, t_token *cur)
{
	t_checker	*chk;
	t_token		*ret;

	chk = data->chk;
	while (cur && cur->type != TOK_EOF)
	{
		ret = check_token(chk, &cur);
		if (ret)
			return (ret);
		if (cur)
			cur = cur->next;
	}
	chk->current = cur;
	return (NULL);
}

int	special_checker(t_main_data *data, t_tokens *tokens)
{
	t_token		*error;

	error = check_tokens(data, init_checker(data->chk, tokens));
	debug_mini_title(LVL_CHK, "Fin du checking");
	if (error)
	{
		if (!error->str)
		{
			if (error->head)
				display_wordpart_error(data, error);
		}
		else
			print_syntax_error(error->str, data->partial_input);
		return (-1);
	}
	if (DLVL == -2 && data->lx->ctxs.count > 0)
	{
		display_unclosed_context(data, error);
		return (-1);
	}
	return (0);
}
