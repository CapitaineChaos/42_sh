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
 */
void	minishell(t_mns *mns)
{
	t_ast_node	*node;

	while (read_user_input(&mns->data))
	{
		if (run_parser(&mns->parser, &mns->lexer.tokens))
		{
			node = (t_ast_node *)mns->parser.deques.final.head->node;
			if (node != NULL)
			{
				run_exec(node);
			}
			parser_free(&mns->parser);
		}
	}
}
