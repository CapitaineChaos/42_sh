/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ops.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:39:46 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/25 02:29:59 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_deque.h"

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
