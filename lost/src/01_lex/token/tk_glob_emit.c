/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tk_glob_emit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 01:11:40 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/24 05:03:48 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_token.h"
#include "module_lexer.h"

t_token	*tk_glob_emit(char *str)
{
	t_tk_part	*part;
	t_token		*token;

	token = token_new(TOK_GLOB);
	part = tk_part_new(TOK_GLOB, 0);
	if (!part)
	{
		free_and_exit_minishell(EXIT_FAILURE);
		return (NULL);
	}
	token->head = part;
	token->tail = part;
	token->count = 1;
	token->str = str;
	return (token);
}
