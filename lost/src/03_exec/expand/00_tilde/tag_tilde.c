/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tag_tilde.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 03:18:06 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 21:51:23 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_expand.h"
#include "module_token.h"

static void	tag_tilde(t_token *token, t_tk_part *part)
{
	t_tk_part	*tmp;

	tmp = part;
	if (tmp->str && tmp->type == TOK_UQUOTE)
	{
		if (tmp->str[0] == '~')
		{
			token->has_tilde = true;
			tmp->has_tilde = true;
		}
	}
}

void	tag_tokens_tildes(t_token *first)
{
	t_tk_part	*part;
	t_token		*token;

	token = first;
	if (token == NULL)
		return ;
	while (token)
	{
		if (is_operand(token->type) && !is_redirect(token->type)
			&& token->type != TOK_DELIM)
		{
			part = token->head;
			while (part)
			{
				if (part->type == TOK_DQUOTE || part->type == TOK_SQUOTE)
					token->has_quoted_part = true;
				tag_tilde(token, part);
				part = part->next;
			}
		}
		token = token->next;
	}
}
