/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tag_dollar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 03:18:06 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 23:01:55 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_expand.h"
#include "module_token.h"
#include "module_debug.h"

static void	tag_heredoc(t_token *token)
{
	if (token->type != TOK_HEREDOC_BODY || token->type == TOK_DELIM)
		return ;
	trace_info(LVL_EXPAND, "Tagging HEREDOC token");
	if (token->type == TOK_HEREDOC_BODY && token->prev)
	{
		trace_info_nvstr(LVL_EXPAND, "Previous token type", debug_get_token_type(token->prev->type));
		trace_info_nvstr(LVL_EXPAND, "Previous token has quoted part", token->prev->has_quoted_part ? "true" : "false");
		if (!token->prev->has_quoted_part)
		{
			token->has_dollar = true;
		}
	}
}

static void	tag_dollar(t_token *token)
{
	t_tk_part	*p;
	int			idx;

	if (token->type == TOK_HEREDOC_BODY || token->type == TOK_DELIM)
		return ;
	p = token->head;
	while (p)
	{
		if (p->str && p->next && (p->type == TOK_UQUOTE)
			&& !ft_strcmp(p->str, "$"))
			p->str[0] = '\0';
		else if (p->str && (p->type == TOK_UQUOTE || p->type == TOK_DQUOTE))
		{
			idx = find_unescaped_char(p->str, '$');
			trace_info_nvnb(LVL_EXPAND, "Unescaped dollar found at index", idx);
			if (idx != -1 && p->str[idx + 1] != 0)
			{
				token->has_dollar = true;
				p->has_dollar = true;
			}
		}
		trace_info_nvstr(LVL_EXPAND, "Token has dollar", token->has_dollar ? "true" : "false");
		p = p->next;
	}
}

static void	tag_token_quotes(t_token *token)
{
	t_tk_part	*part;

	part = token->head;
	while (part)
	{
		trace_info(LVL_EXPAND, "    This token has a part");
		if (part->type == TOK_DQUOTE || part->type == TOK_SQUOTE)
		{
			trace_info(LVL_EXPAND, "          This part is quoted");
			token->has_quoted_part = true;
			break ;
		}
		part = part->next;
	}
}

void	tag_tokens_vars(t_token *first)
{
	t_token		*token;

	token = first;
	if (token == NULL)
		return ;
	trace_info(LVL_EXPAND, "Tagging tokens");
	while (token)
	{
		trace_info_nvstr(LVL_EXPAND, "TAG Token type", debug_get_token_type(token->type));
		if (token->kind == TKD_OPERAND && token->family != TKF_REDIRECT)
		{
			tag_token_quotes(token);
			tag_dollar(token);
			tag_heredoc(token);
		}
		token = token->next;
	}
}
