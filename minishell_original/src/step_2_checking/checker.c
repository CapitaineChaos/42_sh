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

bool	is_bad_redir(t_token **token)
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

bool	is_bad_cmd(t_token **token)
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

t_token	*init_checker(t_checker *chk, t_tokens *tokens)
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

t_token	*c_checke(t_main_data *data, t_token *cur)
{
	t_checker	*chk;
	t_token		*ret;

	chk = data->chk;
	while (cur && cur->type != TOK_EOF)
	{
		ret = inner_checker(chk, &cur);
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

	error = c_checke(data, init_checker(data->chk, tokens));
	debug_mini_title(LVL_CHK, "Fin du checking");
	if (error)
	{
		if (!error->str)
		{
			if (error->head)
				disp_stronly(data, error);
		}
		else
			print_syntax_error(error->str, data->partial_input);
		return (-1);
	}
	if (DLVL == -2 && data->lx->ctxs.count > 0)
	{
		disp_nomultiline(data, error);
		return (-1);
	}
	return (0);
}
