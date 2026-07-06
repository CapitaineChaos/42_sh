/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_build.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 02:27:43 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/25 04:28:35 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_exec.h"
#include "module_parser.h"
#include "module_assembly.h"

size_t	get_cmd_argv_size(t_tokens *tokens)
{
	size_t	i;
	t_token	*tok;

	i = 0;
	tok = tokens->head;
	while (tok)
	{
		if (tok->group == TKG_ARGUMENT && tok->str != NULL)
			i++;
		tok = tok->next;
	}
	return (i);
}

char	**create_cmd_argv(t_tokens *tokens, int *argc)
{
	char	**argv;
	size_t	i;
	t_token	*tok;

	*argc = get_cmd_argv_size(tokens);
	argv = malloc((*argc + 1) * sizeof(char *));
	if (!argv)
		free_and_exit_minishell(EXIT_FAILURE);
	i = 0;
	tok = tokens->head;
	while (tok)
	{
		if (tok->group == TKG_ARGUMENT && tok->str != NULL)
		{
			argv[i] = tok->str;
			i++;
		}
		tok = tok->next;
	}
	argv[i] = NULL;
	return (argv);
}
