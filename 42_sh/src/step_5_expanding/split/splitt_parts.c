/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitt_parts.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 19:30:43 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 21:13:14 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_expand.h"
#include "module_assembly.h"
#include "module_debug.h"
#include "module_token.h"

void	split_part(t_tokens *out, t_token *current, t_tk_part *part)
{
	int			n;
	int			i;
	char		**words;
	t_tk_part	*p;

	words = split_unescaped(part->str, " \t\n", &n);
	i = 0;
	while (i < n)
	{
		if (!*words[i])
			continue ;
		if (!current)
		{
			current = token_new(TOK_WORD);
			tk_list_append(out, current);
		}
		p = tk_part_new(TOK_UQUOTE, 0);
		p->str = strdup(words[i]);
		tk_append_part_tok(current, p);
		if (i + 1 < n)
			current = NULL;
		i++;
	}
	free_split(words);
}

void	do_not_split(t_tokens *out, t_token *current, t_tk_part *part)
{
	t_tk_part	*next;

	if (!current)
	{
		current = token_new(TOK_WORD);
		tk_list_append(out, current);
	}
	next = part->next;
	part->next = NULL;
	part->prev = NULL;
	tk_append_part_tok(current, part);
	if (next)
		part = next->prev;
	else
		part = NULL;
}

/**
 * @brief Pour un seul token `tok`, construit et renvoie une liste de
 * 1 ou plusieurs nouveaux tokens en coupant SES parts UNQUOTED
 * sur les espaces non-échappés.
 */
t_tokens	split_parts(t_token *tok)
{
	t_tokens	out;
	t_token		*current;
	t_tk_part	*part;
	t_tk_part	*next;

	memset(&out, 0, sizeof(t_tokens));
	current = NULL;
	part = tok->head;
	while (part)
	{
		next = part->next;
		if (part->type == TOK_UQUOTE && part->str && part->str[0] == '\0')
			free_token_part(part);
		else if (part_needs_split(part))
			split_part(&out, current, part);
		else
			do_not_split(&out, current, part);
		part = next;
	}
	return (out);
}
