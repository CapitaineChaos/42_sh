/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tk_word_emit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 01:41:09 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/25 23:03:14 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_token.h"
#include "module_lexer.h"

void	transplant_parts(t_token *token, t_tk_part *parts)
{
	t_tk_part	*cur;

	cur = parts;
	if (parts == NULL)
		return ;
	while (cur)
	{
		cur = cur->next;
		token->count++;
	}
	if (token->head == NULL)
		token->head = parts;
	else
	{
		token->tail->next = parts;
		parts->prev = token->tail;
	}
	cur = parts;
	while (cur->next)
		cur = cur->next;
	token->tail = cur;
}

t_token	*new_word_token(t_tk_part *first)
{
	t_token	*token;

	token = token_new(TOK_WORD);
	if (!token)
		free_and_exit_minishell(EXIT_FAILURE);
	token->kind = TKD_OPERAND;
	token->family = TKF_WORD;
	token->group = TKG_ARGUMENT;
	token->head = NULL;
	token->tail = NULL;
	token->count = 0;
	transplant_parts(token, first);
	return (token);
}
	

void	tk_word_emit(t_lexer *lx)
{
	t_token	*token;

	if (lx->tmp_wp == NULL)
	{
		return ;
	}
	token = token_new(TOK_WORD);
	if (!token)
		free_and_exit_minishell(EXIT_FAILURE);
	token->kind = TKD_OPERAND;
	token->family = TKF_WORD;
	token->group = TKG_ARGUMENT;
	transplant_parts(token, lx->tmp_wp);
	lx->tmp_wp = NULL;
	tk_list_append(&lx->tokens, token);
}
