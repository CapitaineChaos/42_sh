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

#include "module_exec.h"
#include "module_parser.h"
#include "module_assembly.h"
#include "module_expand.h"

bool	run_exec(t_ast_node *first, int lv)
{
	int	code;

	if (lv < 7)
		return (false);
	if (first == NULL)
		return (false);
	code = exec_node(first);
	set_code(code);
	return (true);
}

typedef struct s_ast_disp
{
	t_ast_type	type;
	int			(*fn)(t_ast_node *node);
}	t_ast_disp;

/* Correspondance nœud AST -> handler d'exécution. */
static const t_ast_disp	g_ast_exec[] = {
	{AST_PIPE, exec_pipeline},
	{AST_AND, exec_and_or},
	{AST_OR, exec_and_or},
	{AST_SEQ, exec_sequence},
};

int	exec_node(t_ast_node *node)
{
	size_t	i;

	if (!node)
		return (EXIT_SUCCESS);
	if (node->type == AST_SUBSHELL)
		return (exec_subshell(node));
	if (node->tclass == ACL_OPERAND)
		return (exec_command(node));
	for (i = 0; i < sizeof(g_ast_exec) / sizeof(g_ast_exec[0]); i++)
		if (g_ast_exec[i].type == node->type)
			return (g_ast_exec[i].fn(node));
	return (EXIT_FAILURE);
}
