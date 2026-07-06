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

#include "module_debug.h"
#include "ft_deque.h"
#include "ft_std.h"
#include <stdlib.h>

/**
 * @brief Crée un nœud pour la dèque
 * @param foreign_node Le nœud étranger à encapsuler
 * @return Un pointeur vers le nœud créé
 */
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

/**
 * @brief Libère un nœud de la dèque
 * @note Si del est NULL, les données ne seront pas libérées (au choix)
 * @param node Le nœud à libérer
 * @param del La fonction de libération des données
 */
void	deque_node_free(t_dq_n *node, void (*del)(void *))
{
	if (node)
	{
		trace_info(LVL_PARSER, "Freeing node");
		if (node->node && del)
			del(node->node);
		free(node);
	}
}

/**
 * @brief Libère tous les nœuds de la dèque.
 * @note La dèque elle-même n'est pas libérée. Le faire en dehors si besoin.
 * @note Si del est NULL, les données ne seront pas libérées (au choix)
 * @param deque La dèque à libérer
 * @param del La fonction de libération des données
 */
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

/**
 * @brief Initialise la dèque
 * @param deque La dèque à initialiser
 */
void	deque_init(t_deque *deque)
{
	deque->head = NULL;
	deque->tail = NULL;
	deque->size = 0;
}

/**
 * @brief Reset la dèque en libérant tous les nœuds sans libérer les données
 * @param deque La dèque à réinitialiser
 */
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
