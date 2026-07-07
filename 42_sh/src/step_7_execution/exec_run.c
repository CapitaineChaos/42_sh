/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_run.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:34:11 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 00:04:23 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_exec.h"
#include "module_parser.h"
#include "module_assembly.h"
#include "module_expand.h"

/**
 * @brief Lanceur de l'exécution de l'arbre AST.
 * @param mns Le minishell.
 */
bool	run_exec(t_ast_node *first, int lv)
{
	int	code;

	if (lv < 7)
		return (false);
	if (first == NULL)
		return (false);
	code = exec_node(first);
	set_code(code);
	dbg_exec(first);
	dbg_footer(code);
	return (true);
}

/**
 * @brief Exécute un nœud de l'arbre AST.
 * @param node Le nœud de l'arbre AST à exécuter.
 * @return Le code de retour du nœud exécuté.
 */
int	exec_node(t_ast_node *node)
{
	if (!node)
		return (EXIT_SUCCESS);
	if (node->type == AST_SUBSHELL)
		return (exec_subshell(node));
	if (node->tclass == ACL_OPERAND)
		return (exec_command(node));
	if (node->type == AST_PIPE)
		return (exec_pipeline(node));
	if (node->type == AST_AND || node->type == AST_OR)
		return (exec_and_or(node));
	if (node->type == AST_SEQ)
		return (exec_sequence(node));
	return (EXIT_FAILURE);
}
