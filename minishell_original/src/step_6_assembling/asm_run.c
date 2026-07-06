/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_run.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 03:56:20 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/25 03:20:19 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_assembly.h"


int	assemble_node(t_ast_node *node)
{
	t_operand	*op;
	int			argc;

	debug_pico_title(LVL_ARGV, "Assembling node", "\001\033[92m\002");
	if (node == NULL)
		return (EXIT_FAILURE);
	if (node->tclass == ACL_OPERAND)
	{
		op = &node->t_ast_data.operand;
		op->argv = create_cmd_argv(&op->tokens, &argc);
		op->argc = argc;
		trace_info_nvnb(LVL_ARGV, "Assembled argc", op->argc);
	}
	return (EXIT_SUCCESS);
}
