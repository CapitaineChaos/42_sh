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
	if (*token && tok_class((*token)->type) == LCL_REDIR)
	{
		*token = (*token)->next;
		if (!(*token) || (*token)->type == TOK_WORD)
			return (false);
		else
		{
			return (true);
		}
	}
	return (false);
}

static bool	is_bad_cmd(t_token **token)
{
	while (*token && tok_has((*token)->type, TA_CMD_PART))
	{
		if (is_bad_redir(token))
			return (true);
		if (*token && (*token)->next
			&& tok_has((*token)->next->type, TA_CMD_PART))
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
		if ((*cur)->prev && tok_has((*cur)->prev->type, TA_CMD_PART))
		{
			*ret = (*cur)->next;
			if (!(*cur)->prev->prev)
				return (true);
			if (!tok_has((*cur)->prev->prev->type, TA_CMD_PART))
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
	if (*cur && (*cur)->type == TOK_WORD)
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
	if (*cur && tok_class((*cur)->type) == LCL_REDIR)
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
	if (*cur && tok_has((*cur)->type, TA_SEP))
	{
		*ret = *cur;
		if (!chk->cmd_ready)
			return (true);
		set_states(chk, 1, 1, 1);
		*ret = NULL;
		return (true);
	}
	if (*cur && tok_has((*cur)->type, TA_BINARY))
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
		cur = chk->current;
	}
	else
	{
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
	return (0);
}
