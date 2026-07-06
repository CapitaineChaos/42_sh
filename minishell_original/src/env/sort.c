/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 01:04:59 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 21:33:37 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_env.h"
#include "ft_std.h"

int	env_key_cmp(t_env_node *a, t_env_node *b)
{
	return (ft_strcmp(a->key, b->key));
}

static void	env_lst_insert_sorted(t_env_node **sorted, t_env_node *node)
{
	t_env_node	*cur;

	if (!*sorted || env_key_cmp(node, *sorted) < 0)
	{
		node->next = *sorted;
		if (*sorted)
			(*sorted)->prev = node;
		*sorted = node;
	}
	else
	{
		cur = *sorted;
		while (cur->next && env_key_cmp(cur->next, node) < 0)
			cur = cur->next;
		node->next = cur->next;
		if (cur->next)
			cur->next->prev = node;
		cur->next = node;
		node->prev = cur;
	}
}

void	env_lst_reorder(t_env_lst *lst)
{
	t_env_node	*sorted;
	t_env_node	*cur;
	t_env_node	*next;

	if (!lst || !lst->head)
		return ;
	sorted = NULL;
	cur = lst->head;
	while (cur)
	{
		next = cur->next;
		cur->prev = NULL;
		cur->next = NULL;
		env_lst_insert_sorted(&sorted, cur);
		cur = next;
	}
	lst->head = sorted;
	lst->tail = sorted;
	while (lst->tail && lst->tail->next)
		lst->tail = lst->tail->next;
}
