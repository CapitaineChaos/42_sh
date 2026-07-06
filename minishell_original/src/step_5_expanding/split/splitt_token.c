/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitt_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 20:51:00 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 21:00:14 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_expand.h"
#include "module_assembly.h"
#include "module_debug.h"
#include "module_token.h"

static int	split_token(t_tokens *tokens, t_token *cur)
{
	t_tokens	sub;
	t_token		*prev;
	t_token		*next;

	sub = split_parts(cur);
	if (sub.count > 0)
		replace_in_list(tokens, cur, sub.head);
	else
	{
		prev = cur->prev;
		next = cur->next;
		if (prev)
			prev->next = next;
		else
			tokens->head = next;
		if (next)
			next->prev = prev;
		else
			tokens->tail = prev;
		tokens->count--;
		free(cur);
	}
	trace_info_nvnb(LVL_EXPAND, "Splitted token in parts", sub.count);
	return (sub.count);
}

static bool	need_split(t_tk_part *p)
{
	while (p)
	{
		if (part_needs_split(p))
			return (true);
		if (p->type == TOK_UQUOTE && p->str && p->str[0] == '\0')
			return (true);
		p = p->next;
	}
	return (false);
}

/**
 * @brief Parcourt tous les tokens et, pour chaque WORD non-quoted contenant
 * un espace, remplace `cur` par la sous-liste
 * split_token_by_parts(cur).
 */
int	split_tokens(t_tokens *tokens, bool is_redir)
{
	t_token		*cur;
	t_token		*next;
	int			n;

	cur = tokens->head;
	while (cur)
	{
		next = cur->next;
		if (cur->kind == TKD_OPERAND && cur->family == TKF_WORD
			&& !cur->has_quoted_part)
		{
			if (need_split(cur->head))
			{
				n = split_token(tokens, cur);
				if (is_redir && n != 1)
					return (n);
			}
		}
		cur = next;
	}
	return (-1);
}
