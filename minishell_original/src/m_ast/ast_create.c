/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_create.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 01:39:30 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 00:05:18 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_ast.h"
#include "module_minishell.h"
#include "module_parser.h"
#include "module_token.h"
#include "ft_std.h"
#include <stdlib.h>

/**
 * @brief Crée un nœud d'opérande de commande.
 * @param type Le type d'opérande de commande.
 * @return Un pointeur vers le nœud créé.
 */
t_ast_node	*create_cmd_word_operand(t_ast_type type)
{
	t_ast_node	*node;

	node = ft_calloc(1, sizeof(t_ast_node));
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

	node = ft_calloc(1, sizeof(t_ast_node));
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

/**
 * @brief Crée un nœud d'opérateur.
 * @param type Le type d'opérateur.
 * @param token Le token associé au nœud.
 * @return Un pointeur vers le nœud créé.
 */
t_ast_node	*create_operator(t_ast_type type, t_token *token)
{
	t_ast_node	*node;

	node = ft_calloc(1, sizeof(t_ast_node));
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

t_ast_node	*create_subshell(t_ast_node *old)
{
	t_ast_node	*node;
	t_tokens	*tokens;
	t_tokens	*redirections;

	node = ft_calloc(1, sizeof(t_ast_node));
	if (!node)
	{
		free_and_exit_minishell(EXIT_FAILURE);
		return (NULL);
	}
	node->type = AST_SUBSHELL;
	node->tclass = ACL_OPERAND;
	tokens = &old->t_ast_data.operand.tokens;
	redirections = &old->t_ast_data.operand.redirections;
	free_token_list(tokens);
	node->t_ast_data.operand.redirections.head = redirections->head;
	node->t_ast_data.operand.redirections.tail = redirections->tail;
	node->t_ast_data.operand.redirections.count = redirections->count;
	ft_bzero(&old->t_ast_data.operand, sizeof(t_tokens));
	return (node);
}

t_ast_node	*create_struct(t_ast_type type, t_token *token)
{
	t_ast_node	*node;

	node = ft_calloc(1, sizeof(t_ast_node));
	if (!node)
	{
		free_and_exit_minishell(EXIT_FAILURE);
		return (NULL);
	}
	node->type = type;
	node->tclass = get_ast_class(token->type);
	if (node->tclass == ACL_OPERATOR)
	{
		node->t_ast_data.operator_.precedence = get_precedence(type);
		node->t_ast_data.operator_.token = token;
	}
	else
	{
		node->t_ast_data.operand.tokens.head = token;
		node->t_ast_data.operand.tokens.tail = token;
		node->t_ast_data.operand.tokens.count = 1;
	}
	return (node);
}
