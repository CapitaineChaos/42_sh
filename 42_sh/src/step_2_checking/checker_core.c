/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_core.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 13:59:19 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 11:10:34 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_checker.h"
#include "module_token.h"
#include "module_exec.h"

bool	check_lparentheses(t_checker *chk, t_token **cur, t_token **ret)
{
	*ret = NULL;
	if (*cur && (*cur)->type == TOK_LPAREN)
	{
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
	return (false);
}

bool	check_parentheses(t_checker *chk, t_token **cur, t_token **ret)
{
	*ret = NULL;
	if (check_lparentheses(chk, cur, ret))
	{
		if (*ret)
			return (true);
		return (true);
	}
	else if (*cur && (*cur)->type == TOK_RPAREN)
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

bool	check_cmd(t_checker *chk, t_token **cur, t_token **ret)
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
	else if (*cur && (*cur)->family == TKF_REDIRECT)
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

bool	check_else(t_checker *chk, t_token **cur, t_token **ret)
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
	else if (*cur && (*cur)->family == TKF_OPERATOR)
	{
		*ret = *cur;
		if (!chk->cmd_ready)
			return (true);
		set_states(chk, 0, 0, 1);
		*ret = NULL;
		return (true);
	}
	else
	{
		*ret = *cur;
		return (true);
	}
	return (false);
}

t_token	*inner_checker(t_checker *chk, t_token **cur)
{
	t_token	*ret;

	ret = NULL;
	trace_info_nvstr(LVL_CHK, "Current token", debug_get_token_type((*cur)->type));
	if ((*cur)->type == TOK_NEWLINE)
		set_states(chk, 0, 1, 1);
	else if (check_parentheses(chk, cur, &ret))
	{
		if (ret)
			return (ret);
	}
	else if (check_cmd(chk, cur, &ret))
	{
		if (ret)
			return (ret);
	}
	else if (check_else(chk, cur, &ret))
	{
		if (ret)
			return (ret);
	}
	return (ret);
}
