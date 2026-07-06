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
#include "module_debug.h"

/**
 * @brief Pop une node de la tête de la deque.
 * @note Maintient la tête, la queue et la taille de la deque.
 * @note La node popée n'est pas free.
 * @param stack La deque à manipuler.
 * @return La node popée.
 */
t_dq_n	*deque_pop_front(t_deque *deque)
{
	t_dq_n	*node;

	trace_info_nvstr(LVL_PARSER, "Popping node from front", deque->name);
	trace_info_nvstr(LVL_PARSER, "Node type               ", get_ast_typestr(((t_ast_node *)deque->head->node)->type));
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

/**
 * @brief Push une node à la tête de la deque.
 * @note Maintient la tête, la queue et la taille de la deque.
 * @param stack La deque à manipuler.
 * @param node La node à push.
 */
void	deque_push_front(t_deque *deque, t_dq_n *node)
{
	if (!node)
	{
		trace_info_nvstr(LVL_PARSER, "Cannot push NULL node to front", deque->name);
		return ;
	}
	trace_info_nvstr(LVL_PARSER, "Pushing node to front", deque->name);
	trace_info_nvstr(LVL_PARSER, "Node type               ", get_ast_typestr(((t_ast_node *)node->node)->type));
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

/**
 * @brief Pop une node de la queue de la deque.
 * @note Maintient la tête, la queue et la taille de la deque.
 * @note La node popée n'est pas free.
 * @param stack La deque à manipuler.
 * @return La node popée.
 */
t_dq_n	*deque_pop_back(t_deque *deque)
{
	t_dq_n	*node;

	trace_info_nvstr(LVL_PARSER, "Popping node from back", deque->name);
	trace_info_nvstr(LVL_PARSER, "Node type               ", get_ast_typestr(((t_ast_node *)deque->tail->node)->type));
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

/**
 * @brief Push une node à la queue de la deque.
 * @note Maintient la tête, la queue et la taille de la deque.
 * @param stack La deque à manipuler.
 * @param node La node à push.
 */
void	deque_push_back(t_deque *dq, t_dq_n *node)
{
	if (!node)
	{
		trace_info_nvstr(LVL_PARSER, "Cannot push NULL node to back", dq->name);
		return ;
	}
	trace_info_nvstr(LVL_PARSER, "Pushing node to back", dq->name);
	trace_info_nvstr(LVL_PARSER, "Node type               ", get_ast_typestr(((t_ast_node *)node->node)->type));
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
