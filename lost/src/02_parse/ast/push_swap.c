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

#include "module_ast.h"
#include "module_minishell.h"
#include "ft_std.h"

void	ps_push_front_create(t_deque *deque, t_ast_node *token)
{
	t_dq_n	*cell;

	if (!token)
		return ;
	cell = (t_dq_n *)calloc(1, sizeof(*cell));
	if (!cell)
	{
		free_and_exit_minishell(EXIT_FAILURE);
		return ;
	}
	cell->node = (void *)token;
	deque_push_front(deque, cell);
}

void	ps_push_back_create(t_deque *deque, t_ast_node *token)
{
	t_dq_n	*cell;

	if (!token)
		return ;
	cell = (t_dq_n *)calloc(1, sizeof(*cell));
	if (!cell)
	{
		free_and_exit_minishell(EXIT_FAILURE);
		return ;
	}
	cell->node = (void *)token;
	deque_push_back(deque, cell);
}

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
