/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tk_glob_emit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 01:11:40 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/31 22:02:19 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_token.h"
#include "module_lexer.h"

/**
 * @brief Émet un token de type glob avec la chaîne donnée.
 */
t_token	*tk_glob_emit(char *str)
{
	t_tk_part	*part;
	t_token		*token;

	token = token_new(TOK_WORD);
	part = tk_part_new(PART_GLOB, 0);
	if (!part)
	{
		free_and_exit_minishell(EXIT_FAILURE);
		return (NULL);
	}
	token->role = TKR_ARGUMENT;
	token->head = part;
	token->tail = part;
	token->count = 1;
	token->str = strdup(str);
	return (token);
}
