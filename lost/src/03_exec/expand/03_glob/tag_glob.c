/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tag_glob.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 03:18:06 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 21:49:11 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_expand.h"
#include "module_token.h"

static void	tag_glob(t_token *token, t_tk_part *part)
{
	t_tk_part	*tmp;

	tmp = part;
	while (tmp)
	{
		if (tmp->str && tmp->type == TOK_UQUOTE)
		{
			if (find_unescaped_char(tmp->str, '*') != -1)
			{
				token->has_glob = true;
				tmp->has_glob = true;
			}
		}
		tmp = tmp->next;
	}
}

void	tag_tokens_globs(t_token *first)
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
				tag_glob(token, part);
				part = part->next;
			}
		}
		token = token->next;
	}
}
