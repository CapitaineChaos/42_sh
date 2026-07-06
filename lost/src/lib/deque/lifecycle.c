/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lifecycle.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:17:05 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/23 03:19:22 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_deque.h"
#include "ft_std.h"
#include <stdlib.h>

t_dq_n	*dstn_new(void *foreign_node)
{
	t_dq_n	*new_node;

	new_node = (t_dq_n *)calloc(1, sizeof(t_dq_n));
	if (!new_node)
		return (NULL);
	new_node->node = foreign_node;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

void	deque_node_free(t_dq_n *node, void (*del)(void *))
{
	if (node)
	{
		if (node->node && del)
			del(node->node);
		free(node);
	}
}

void	deque_free(t_deque *deque, void (*del)(void *))
{
	t_dq_n	*node;
	t_dq_n	*next;

	if (!deque)
		return ;
	node = deque->head;
	while (node)
	{
		next = node->next;
		deque_node_free(node, del);
		node = next;
	}
	deque->head = NULL;
	deque->tail = NULL;
	deque->size = 0;
}

void	deque_init(t_deque *deque)
{
	deque->head = NULL;
	deque->tail = NULL;
	deque->size = 0;
}

void	dstk_reset(t_deque *deque)
{
	t_dq_n	*node;

	while (deque->head)
	{
		node = deque->head;
		deque->head = deque->head->next;
		deque_node_free(node, NULL);
	}
	deque->head = NULL;
	deque->tail = NULL;
	deque->size = 0;
}
