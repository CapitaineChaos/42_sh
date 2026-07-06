/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:48:54 by cmassol           #+#    #+#             */
/*   Updated: 2025/05/28 02:26:46 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_minishell.h"
#include "module_read.h"
#include "module_lexer.h"
#include "module_parser.h"
#include "module_expand.h"
#include "module_assembly.h"
#include "module_exec.h"
#include "module_env.h"
#include "module_checker.h"
#include "module_ast.h"

/**
 * @brief Fonction principale du minishell.
 * 
 * @note 1 : input
 * @note 2 : checker
 * @note 3 : lexer
 * @note 4 : parser
 * @note 5 : expansion
 * @note 6 : assembling
 * @note 7 : execution
 */
void	minishell(t_mns *mns, int lv)
{
	t_ast_node	*node;

	while (read_user_input(&mns->data))
	{
		if (run_parser(&mns->parser, &mns->lexer.tokens, lv))
		{
			node = (t_ast_node *)mns->parser.deques.final.head->node;
			if (node != NULL)
			{
				run_exec(node, lv);
			}
			parser_free(&mns->parser, lv);
		}
		trace_info(LVL_MNS, "  Fin boucle REPL  ");
	}
	trace_info(LVL_MNS, "  Sortie de la boucle REPL  ");
}
