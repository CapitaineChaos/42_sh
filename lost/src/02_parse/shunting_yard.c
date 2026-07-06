/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shunting_yard.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:15:09 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/25 03:20:01 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "texts.h"
#include "module_token.h"
#include "module_parser.h"
#include "ft_deque.h"

static void	pop_back_until_end(t_pdeques *deques)
{
	t_dq_n		*node;

	while (deques->operators.size > 0)
	{
		node = deque_pop_back(&deques->operators);
		deque_push_back(&deques->output, node);
	}
}

static bool	pop_back_until_mark(t_pdeques *deques)
{
	t_dq_n		*node;
	t_ast_node	*tail;

	while (deques->operators.size > 0)
	{
		tail = peek_tail(&deques->operators);
		if (tail->type == AST_SUBSH_MARK)
			return (true);
		node = deque_pop_back(&deques->operators);
		deque_push_back(&deques->output, node);
	}
	return (false);
}

static void	manage_struct(t_pdeques *deques, t_dq_n	*node)
{
	t_dq_n		*popped;
	t_ast_node	*ast_node;
	t_ast_node	*subshell;

	ast_node = get_ast_node(node);
	if (ast_node->type == AST_SUBSH_MARK)
	{
		deque_push_back(&deques->operators, node);
		return ;
	}
	if (ast_node->type == AST_SUBSH_END)
	{
		if (pop_back_until_mark(deques))
		{
			popped = deque_pop_back(&deques->operators);
			deque_node_free(popped, (void (*)(void *))free_ast_node);
			subshell = create_subshell((t_ast_node *)node->node);
			free(node->node);
			node->node = (t_dq_n *)subshell;
			deque_push_back(&deques->output, node);
			return ;
		}
	}
}

static void	manage_op(t_pdeques *deques, t_dq_n	*node)
{
	t_ast_node	*tail;
	t_dq_n		*popped;
	size_t		prec_1;
	size_t		prec_2;

	prec_1 = get_ast_node(node)->t_ast_data.operator_.precedence;
	while (deques->operators.size > 0)
	{
		tail = peek_tail(&deques->operators);
		prec_2 = tail->t_ast_data.operator_.precedence;
		if (prec_1 <= prec_2)
		{
			popped = deque_pop_back(&deques->operators);
			deque_push_back(&deques->output, popped);
		}
		else
			break ;
	}
	deque_push_back(&deques->operators, node);
}

bool	apply_shunting_yard(t_pdeques *deques)
{
	t_ast_node	*node;
	t_dq_n		*popped;

	if (deques->input.size == 0)
		return (false);
	while (deques->input.size > 0)
	{
		popped = deque_pop_back(&deques->input);
		node = get_ast_node(popped);
		if (node->type == AST_SUBSH_END || node->type == AST_SUBSH_MARK)
			manage_struct(deques, popped);
		else if (node->tclass == ACL_OPERAND)
			deque_push_back(&deques->output, popped);
		else if (node->tclass == ACL_OPERATOR || node->tclass == ACL_CONTROL)
			manage_op(deques, popped);
		else
		{
			free_ast_node(node);
		}
	}
	pop_back_until_end(deques);
	return (true);
}
