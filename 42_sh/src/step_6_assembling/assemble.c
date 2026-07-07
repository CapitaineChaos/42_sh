/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assemble.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 02:27:43 by smaitre           #+#    #+#             */
/*   Updated: 2026/07/06 00:00:00 by codex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_exec.h"
#include "module_parser.h"
#include "module_assembly.h"

static size_t	count_argv_tokens(t_tokens *tokens)
{
	size_t	count;
	t_token	*tok;

	count = 0;
	tok = tokens->head;
	while (tok)
	{
		if (tok->role == TKR_ARGUMENT && tok->str != NULL)
			count++;
		tok = tok->next;
	}
	return (count);
}

static char	**create_cmd_argv(t_tokens *tokens, int *argc)
{
	char	**argv;
	size_t	i;
	t_token	*tok;

	*argc = count_argv_tokens(tokens);
	argv = malloc((*argc + 1) * sizeof(char *));
	if (!argv)
		free_and_exit_minishell(EXIT_FAILURE);
	i = 0;
	tok = tokens->head;
	while (tok)
	{
		if (tok->role == TKR_ARGUMENT && tok->str != NULL)
		{
			argv[i++] = tok->str;
		}
		tok = tok->next;
	}
	argv[i] = NULL;
	return (argv);
}

int	assemble_node(t_ast_node *node)
{
	t_operand	*op;
	int			argc;

	if (node == NULL)
		return (EXIT_FAILURE);
	if (node->tclass == ACL_OPERAND)
	{
		op = &node->t_ast_data.operand;
		op->argv = create_cmd_argv(&op->tokens, &argc);
		op->argc = argc;
	}
	return (EXIT_SUCCESS);
}
