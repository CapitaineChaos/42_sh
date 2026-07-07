/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deque.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:39:46 by smaitre           #+#    #+#             */
/*   Updated: 2026/07/06 00:00:00 by codex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "ft_deque.h"
#include "ft_std.h"

t_dq_n	*dstn_new(void *foreign_node)
{
	t_dq_n	*new_node;

	new_node = calloc(1, sizeof(t_dq_n));
	if (!new_node)
		return (NULL);
	new_node->node = foreign_node;
	return (new_node);
}

void	deque_node_free(t_dq_n *node, void (*del)(void *))
{
	if (!node)
		return ;
	if (node->node && del)
		del(node->node);
	free(node);
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

t_dq_n	*deque_pop_front(t_deque *deque)
{
	t_dq_n	*node;

	if (deque->size == 0)
		return (NULL);
	node = deque->head;
	deque->head = deque->head->next;
	if (deque->head)
		deque->head->prev = NULL;
	else
		deque->tail = NULL;
	deque->size--;
	return (node);
}

void	deque_push_front(t_deque *deque, t_dq_n *node)
{
	if (!node)
	{
		return ;
	}
	if (deque->size == 0)
	{
		deque->head = node;
		deque->tail = node;
		node->prev = NULL;
		node->next = NULL;
	}
	else
	{
		node->next = deque->head;
		node->prev = NULL;
		deque->head->prev = node;
		deque->head = node;
	}
	deque->size++;
}

t_dq_n	*deque_pop_back(t_deque *deque)
{
	t_dq_n	*node;

	if (deque->size == 0)
		return (NULL);
	node = deque->tail;
	deque->tail = deque->tail->prev;
	if (deque->tail)
		deque->tail->next = NULL;
	else
		deque->head = NULL;
	deque->size--;
	return (node);
}

void	deque_push_back(t_deque *dq, t_dq_n *node)
{
	if (!node)
	{
		return ;
	}
	if (dq->size == 0)
	{
		node->prev = NULL;
		node->next = NULL;
		dq->head = node;
		dq->tail = node;
	}
	else
	{
		node->prev = dq->tail;
		node->next = NULL;
		dq->tail->next = node;
		dq->tail = node;
	}
	dq->size++;
}
