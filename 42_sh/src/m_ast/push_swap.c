/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:39:46 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/23 02:06:19 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_ast.h"
#include "module_minishell.h"
#include "ft_std.h"

/**
 * @brief Crée et push un nœud dans la deque.
 * @note Absorbe le token associé dans un malloc.
 * @param deque La deque dans laquelle le nœud sera pushé.
 * @param token Le token associé au nœud.
 */
void	ps_push_front_create(t_deque *deque, t_ast_node *token)
{
	t_dq_n	*cell;

	if (!token)
		return ;
	cell = (t_dq_n *)ft_calloc(1, sizeof(*cell));
	if (!cell)
	{
		free_and_exit_minishell(EXIT_FAILURE);
		return ;
	}
	cell->node = (void *)token;
	deque_push_front(deque, cell);
}

/**
 * @brief Crée et push un nœud dans la deque.
 * @note Absorbe le token associé dans un malloc.
 * @param deque La deque dans laquelle le nœud sera pushé.
 * @param token Le token associé au nœud.
 */
void	ps_push_back_create(t_deque *deque, t_ast_node *token)
{
	t_dq_n	*cell;

	if (!token)
		return ;
	cell = (t_dq_n *)ft_calloc(1, sizeof(*cell));
	if (!cell)
	{
		free_and_exit_minishell(EXIT_FAILURE);
		return ;
	}
	cell->node = (void *)token;
	deque_push_back(deque, cell);
}

/**
 * @brief Pop un nœud de la tête de la deque et renvoie son contenu.
 * @note La node popée est free, son contenu est renvoyé
 * @param deque La deque à manipuler.
 * @return La node popée.
 */
t_ast_node	*ps_pop_front_release(t_deque *deque)
{
	t_ast_node	*node;
	t_dq_n		*popped;

	if (deque->size == 0)
		return (NULL);
	popped = deque_pop_front(deque);
	node = (t_ast_node *)popped->node;
	deque_node_free(popped, NULL);
	return (node);
}

/**
 * @brief Pop un nœud de la queue de la deque et renvoie son contenu.
 * @note La node popée est free, son contenu est renvoyé
 * @param deque La deque à manipuler.
 * @return La node popée.
 */
t_ast_node	*ps_pop_back_release(t_deque *deque)
{
	t_ast_node	*node;
	t_dq_n		*popped;

	if (deque->size == 0)
		return (NULL);
	popped = deque_pop_back(deque);
	node = (t_ast_node *)popped->node;
	deque_node_free(popped, NULL);
	return (node);
}
