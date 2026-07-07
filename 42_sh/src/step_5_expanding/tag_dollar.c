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
	if (token->type != TOK_HEREDOC_BODY)
		return ;
	if (token->type == TOK_HEREDOC_BODY && token->prev)
	{
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

	if (token->type == TOK_HEREDOC_BODY
		|| token->role == TKR_HEREDOC_DELIM)
		return ;
	p = token->head;
	while (p)
	{
		if (p->str && p->next && (p->type == PART_UQUOTE)
			&& !strcmp(p->str, "$"))
			p->str[0] = '\0';
		else if (p->str && (p->type == PART_UQUOTE || p->type == PART_DQUOTE))
		{
			idx = find_unescaped_char(p->str, '$');
			if (idx != -1 && p->str[idx + 1] != 0)
			{
				token->has_dollar = true;
				p->has_dollar = true;
			}
		}
		p = p->next;
	}
}

static void	tag_token_quotes(t_token *token)
{
	t_tk_part	*part;

	part = token->head;
	while (part)
	{
		if (part->type == PART_DQUOTE || part->type == PART_SQUOTE)
		{
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
	while (token)
	{
		if (tok_has(token->type, TF_OPERAND)
			&& token->role != TKR_REDIR_OP)
		{
			tag_token_quotes(token);
			tag_dollar(token);
			tag_heredoc(token);
		}
		token = token->next;
	}
}
