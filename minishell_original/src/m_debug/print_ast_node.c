/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 14:15:54 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 20:37:17 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_ast.h"
#include "ft_std.h"
#include "utils.h"

void	debug_ast_node_infos(t_ast_node *node, char *sender)
{
	t_logger	lg;

	log_init(&lg);
	log_puts(&lg, "\n==== AST Node Info (");
	log_puts(&lg, sender);
	log_puts(&lg, ") ====\n");
	log_flush(-1, &lg, true);
	if (!node)
		trace_info(LVL_EXEC, "AST node is NULL");
	trace_info_nvnb(LVL_EXEC, "node->id", node->id);
	if (node->left)
		trace_info_nvstr(LVL_EXEC, "node->left->type", get_ast_typestr(node->left->type));
	else
		trace_info_nvstr(LVL_EXEC, "node->left->type", "NULL");
	if (node->right)	
		trace_info_nvstr(LVL_EXEC, "node->right->type", get_ast_typestr(node->right->type));
	else
		trace_info_nvstr(LVL_EXEC, "node->right->type", "NULL");
	if (node->parent)
		trace_info_nvstr(LVL_EXEC, "node->parent->type", get_ast_typestr(node->parent->type));
	else
		trace_info_nvstr(LVL_EXEC, "node->parent->type", "NULL");
	log_init(&lg);
	log_puts(&lg, "==========================================================\n\n");
	log_flush(-1, &lg, true);
}
