/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tk_ctrl_emit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 20:48:22 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/25 23:03:14 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_token.h"
#include "module_read.h"
#include "utils.h"

void	tk_operator_emit(t_lexer *lx, char *s, t_tk_type type)
{
	t_token	*token;

	token = token_new(type);
	if (!token)
	{
		free_and_exit_minishell(EXIT_FAILURE);
		return ;
	}
	token->role = TKR_NONE;
	token->str = strdup(s);
	tk_list_append(&lx->tokens, token);
}

void	tk_control_emit(t_lexer *lx, char *s, t_tk_type type)
{
	t_token	*token;

	token = token_new(type);
	if (!token)
	{
		free_and_exit_minishell(EXIT_FAILURE);
		return ;
	}
	token->role = TKR_NONE;
	token->str = strdup(s);
	tk_list_append(&lx->tokens, token);
}
