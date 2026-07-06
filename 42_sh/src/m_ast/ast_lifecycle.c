/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_lifecycle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 20:28:32 by smaitre           #+#    #+#             */
/*   Updated: 2025/06/01 01:39:16 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_ast.h"
#include "module_token.h"
#include <stdlib.h>

/**
 * @brief Libère un nœud de l'arbre AST.
 * @param node Le nœud à libérer.
 * @note  Les sous parties de argv sont libérées ailleurs car c'est un stream
 *        qui a été construit à partir de tokens. Donc simple free.
 */
void	free_ast_node(t_ast_node *node)
{
	trace_info(LVL_AST, "Freeing AST node");
	if (!node)
		return ;
	if (node->tclass == ACL_OPERAND)
	{
		free_token_list(&node->t_ast_data.operand.tokens);
		free_token_list(&node->t_ast_data.operand.redirections);
		trace_info(LVL_AST, "Freeing operand argv");
		if (node->t_ast_data.operand.argv != NULL)
			free(node->t_ast_data.operand.argv);
		node->t_ast_data.operand.argv = NULL;
		trace_info(LVL_AST, "Freeing operand envp");
		if (node->t_ast_data.operand.envp != NULL)
			free_char_array(node->t_ast_data.operand.envp);
		node->t_ast_data.operand.envp = NULL;
		trace_info(LVL_AST, "Freeing operand path");
		if (node->t_ast_data.operand.path != NULL)
			free(node->t_ast_data.operand.path);
		trace_info(LVL_AST, "Operand path freed");
		node->t_ast_data.operand.path = NULL;
	}
	else
	{
		free_token(node->t_ast_data.operator_.token);
		node->t_ast_data.operator_.token = NULL;
	}
	trace_info(LVL_AST, "AST node freed");
	free(node);
}

/**
 * @note Argv est construit à partir des str des tokens
 * qui ont été libérées dans free_token_list.
 */
void	free_ast(t_ast_node *node)
{
	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	free_ast_node(node);
}
