/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:43:33 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/25 03:23:23 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_ast.h"

bool	build_operator(t_ast_node *out, t_deque *final)
{
	t_tk_kind	tok_kind;
	t_ast_node	*popped_final;

	if (out->tclass == ACL_OPERATOR || out->tclass == ACL_CONTROL)
	{
		tok_kind = out->t_ast_data.operator_.token->kind;
		if (tok_kind == TKD_OPERATOR)
		{
			popped_final = ps_pop_back_release(final);
			trace_info(LVL_PARSER, "Popping two nodes (Operator found)");
			trace_info_nvstr(LVL_PARSER, "Popped node", get_ast_typestr(popped_final->type));
			out->right = popped_final;
			if (final->size > 0 && out->arity == 2)
			{				
				popped_final = ps_pop_back_release(final);
				trace_info_nvstr(LVL_PARSER, "Popped node", get_ast_typestr(popped_final->type));
				out->left = popped_final;
			}
		}
		#pragma region "DEBUG"
		if (tok_kind != TKD_OPERATOR)
		{
			trace_info(LVL_PARSER, "Unknown operator found");
		}
		#pragma endregion
		return (true);
	}
	return (false);
}

bool	build_subshell(t_ast_node *out, t_deque *final)
{
	t_ast_node	*popped_final;

	if (out->type == AST_SUBSHELL)
	{
		trace_info(LVL_PARSER, "Popping one node (Subshell found)");
		trace_info_nvstr(LVL_PARSER, "Popped node", get_ast_typestr(out->type));
		popped_final = ps_pop_back_release(final);
		out->left = popped_final;
		out->right = NULL;
		return (true);
	}
	return (false);
}

void	build_operand(t_ast_node *out)
{
	trace_info(LVL_PARSER, "Popping nothing (Operand found)");
	out->left = NULL;
	out->right = NULL;
	out->parent = NULL;
}

static void	assign_internal(t_ast_node *node, t_ast_node *parent, int *id)
{
	if (!node)
		return ;
	node->parent = parent;
	node->id = (*id)++;
	assign_internal(node->left, node, id);
	assign_internal(node->right, node, id);
}

/**
 * @brief Construit l'arbre d'AST à partir d'une liste plate RPN de nœuds.
 * @param final La liste de nœuds AST à construire.
 * @param output La liste de nœuds RPN à utiliser pour construire l'AST.
 * @details Parcourt la liste de nœuds RPN et construit l'AST en fonction des
 * opérateurs, des sous-shells et des opérandes trouvés. Les nœuds sont
 * empilés dans la liste finale au fur et à mesure de leur traitement.
 * Les opérateurs et les sous-shells sont traités en reliant les nœuds
 * adjacents, tandis que les opérandes sont simplement ajoutées à l'arbre.
 * Les nœuds sont ensuite ajoutés à la liste finale.
 */
void	build_ast(t_deque *final, t_deque *output)
{
	t_dq_n		*popped_out;
	t_ast_node	*root;
	t_ast_node	*out;
	int			start_id;

	deque_init(final);
	debug_mini_title(LVL_PARSER, "Build AST");
	if (output->size == 0)
		return ;
	while (output->size > 0)
	{
		popped_out = deque_pop_front(output);
		out = (t_ast_node *)popped_out->node;
		if (build_subshell(out, final))
			;
		else if (build_operator(out, final))
			;
		else
			build_operand(out);
		deque_push_back(final, popped_out);
	}
	root = (t_ast_node *)final->tail->node;
	start_id = 0;
	assign_internal(root, NULL, &start_id);
}
