/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_and_or.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:34:27 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/23 23:47:47 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_exec.h"
#include "module_parser.h"
#include <sys/wait.h>

/**
 * @brief Exécute un nœud de type AND ou OR.
 * @param node Le nœud de l'arbre AST à exécuter.
 * @return Le code de retour du nœud exécuté.
 */
int	exec_and_or(t_ast_node *node)
{
	int	status;

	status = exec_node(node->left);
	if ((node->type == AST_AND && status == 0) || (
			node->type == AST_OR && status != 0))
		return (exec_node(node->right));
	return (status);
}
