/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast_tree.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:12:51 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 20:37:17 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_parser.h"
#include "module_debug.h"
#include "ft_deque.h"
#include "utils.h"

/*
* Affiche une indentation de 'n' espaces
*/
static void	print_indent(int indent)
{
	int			i;
	t_logger	lg;

	log_init(&lg);
	i = 0;	
	while (i < indent)
	{
		log_puts(&lg, " ");
		++i;
	}
	log_flush(-1, &lg, true);
}

/*
* Affiche l'AST avec indentations
*/
void	debug_ast_node(t_ast_node *node, int indent)
{
	t_logger	lg;

	log_init(&lg);
	if (!node)
		return ;
	print_indent(indent);
	log_puts(&lg, "Node ");
	log_puts(&lg, get_ast_typestr(node->type));
	if (node->left != NULL)
		log_puts(&lg, "+L ");
	else
		log_puts(&lg, "-L ");
	if (node->right != NULL)
		log_puts(&lg, "+R ");
	else
		log_puts(&lg, "-R ");
	log_puts(&lg, "\n");
	log_flush(-1, &lg, true);
	debug_ast_node(node->left, indent + 2);
	debug_ast_node(node->right, indent + 2);
}

void	debug_ast_tree(t_deque *deque)
{
	t_ast_node	*node;
	t_logger	lg;

	if (DLVL < LVL_PARSER)
		return ;
	trace_short_info_no_nl(LVL_PARSER, "\n\n=========== Printing AST tree ===========\n");
	log_init(&lg);
	log_puts(&lg, "AST tree (size: ");
	log_putlong(&lg, deque->size);
	log_puts(&lg, ")\n");
	log_flush(-1, &lg, true);
	trace_short_info_no_nl(LVL_PARSER, "------------------------------------------\n");
	node = peek_head(deque);
	debug_ast_node(node, 0);
	trace_short_info_no_nl(LVL_PARSER, "============ End of AST tree ============\n\n");
}
