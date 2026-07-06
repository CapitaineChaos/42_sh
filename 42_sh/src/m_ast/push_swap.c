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
 * @brief Crée et push un nœud en tête de la deque.
 * @note Enveloppe le nœud AST dans une cellule allouée.
 * @param deque La deque dans laquelle le nœud sera pushé.
 * @param token Le nœud AST à envelopper.
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
