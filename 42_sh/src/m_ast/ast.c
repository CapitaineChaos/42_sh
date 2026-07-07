/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 01:39:30 by smaitre           #+#    #+#             */
/*   Updated: 2026/07/06 00:00:00 by codex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_ast.h"
#include "module_debug.h"
#include "module_minishell.h"
#include "module_parser.h"
#include "module_token.h"
#include "ft_std.h"

t_ast_node	*create_cmd_word_operand(t_ast_type type)
{
	t_ast_node	*node;

	node = calloc(1, sizeof(t_ast_node));
	if (!node)
	{
		free_and_exit_minishell(EXIT_FAILURE);
		return (NULL);
	}
	node->type = type;
	node->tclass = ACL_OPERAND;
	return (node);
}

t_ast_node	*create_control(t_ast_type type, t_token *token)
{
	t_ast_node	*node;

	node = calloc(1, sizeof(t_ast_node));
	if (!node)
	{
		free_and_exit_minishell(EXIT_FAILURE);
		return (NULL);
	}
	node->type = type;
	node->tclass = ACL_CONTROL;
	node->arity = 2;
	node->t_ast_data.operator_.precedence = get_precedence(type);
	node->t_ast_data.operator_.token = token;
	return (node);
}

t_ast_node	*create_operator(t_ast_type type, t_token *token)
{
	t_ast_node	*node;

	node = calloc(1, sizeof(t_ast_node));
	if (!node)
	{
		free_and_exit_minishell(EXIT_FAILURE);
		return (NULL);
	}
	node->type = type;
	node->tclass = ACL_OPERATOR;
	node->arity = 2;
	node->t_ast_data.operator_.precedence = get_precedence(type);
	node->t_ast_data.operator_.token = token;
	return (node);
}

t_ast_node	*peek_head(t_deque *deque)
{
	if (deque->size == 0)
		return (NULL);
	return ((t_ast_node *)deque->head->node);
}

static void	free_ast_node(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->tclass == ACL_OPERAND)
	{
		free_token_list(&node->t_ast_data.operand.tokens);
		free_token_list(&node->t_ast_data.operand.redirections);
		free(node->t_ast_data.operand.argv);
		node->t_ast_data.operand.argv = NULL;
		node->t_ast_data.operand.envp = NULL;
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
