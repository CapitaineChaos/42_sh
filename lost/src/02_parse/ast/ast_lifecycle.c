/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_lifecycle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 20:28:32 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/25 07:16:35 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_ast.h"
#include "module_token.h"
#include <stdlib.h>

void	free_ast_node(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->tclass == ACL_OPERAND)
	{
		free_token_list(&node->t_ast_data.operand.tokens);
		free_token_list(&node->t_ast_data.operand.redirections);
		if (node->t_ast_data.operand.argv != NULL)
			free(node->t_ast_data.operand.argv);
		node->t_ast_data.operand.argv = NULL;
		if (node->t_ast_data.operand.envp != NULL)
			free_char_array(node->t_ast_data.operand.envp);
		node->t_ast_data.operand.envp = NULL;
		if (node->t_ast_data.operand.path != NULL)
			free(node->t_ast_data.operand.path);
		node->t_ast_data.operand.path = NULL;
	}
	else
	{
		free_token(node->t_ast_data.operator_.token);
		node->t_ast_data.operator_.token = NULL;
	}
	free(node);
}

void	free_ast(t_ast_node *node)
{
	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	free_ast_node(node);
}
