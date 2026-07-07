/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aggregate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 22:33:42 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 03:43:48 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_config.h"
#include "ft_strapi.h"
#include "ft_std.h"
#include "module_expand.h"
#include "module_minishell.h"


static void	aggregate_tokens(t_tokens *tk, t_sstr **acc)
{
	t_token		*current;
	t_tk_part	*part;
	char		*frag;

	current = tk->head;
	if (current == NULL)
		return ;
	while (current)
	{
		part = current->head;
		while (part)
		{
			frag = part->stream;
			if (frag)
			{
				sstrs_append(acc, strdup(frag));
			}
			part = part->next;
		}
		current = current->next;
	}
	return ;
}

char	*aggregate_cmdline(t_tokens *cmds, t_tokens *redirs)
{
	char	*total;
	t_sstr	*acc;

	acc = NULL;
	if (cmds && cmds->head)
		aggregate_tokens(cmds, &acc);
	if (redirs && redirs->head)
		aggregate_tokens(redirs, &acc);
	if (acc == NULL)
		return (strdup("(Background process)"));
	total = sstrs_flatten_sep(acc, ' ');
	sstr_free(&acc);
	return (total);
}

char	*aggregate_final_token_parts(t_tk_part *first)
{
	t_tk_part	*part;
	t_sstr		*acc;
	char		*frag;

	if (first == NULL)
		return (NULL);
	part = first;
	acc = NULL;
	while (part)
	{
		frag = part->str;
		if (frag)
			sstrs_append(&acc, strdup(frag));
		part = part->next;
	}
	if (acc == NULL)
		return (NULL);
	frag = sstrs_flatten(acc);
	sstr_free(&acc);
	free(acc);
	return (frag);
}

void	aggregate_token_list(t_token *first)
{
	t_token	*current;

	if (first == NULL)
		return ;
	current = first;
	while (current)
	{
		if (current->type == TOK_WORD)
		{
			if (current->count > 0)
				current->str = aggregate_final_token_parts(current->head);
		}
		current = current->next;
	}
}
