/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_append.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 01:39:31 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/27 00:35:50 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_token.h"

t_token	*tk_list_pop_front(t_tokens *tokens)
{
	t_token	*token;

	if (tokens->head == NULL)
		return (NULL);
	token = tokens->head;
	tokens->head = tokens->head->next;
	if (tokens->head)
		tokens->head->prev = NULL;
	else
		tokens->tail = NULL;
	token->next = NULL;
	token->prev = NULL;
	tokens->count--;
	return (token);
}

static void	tk_append(t_token **head, t_token *tok)
{
	if (*head == NULL)
	{
		*head = tok;
		tok->next = NULL;
		tok->prev = NULL;
	}
	else
	{
		tok->next = *head;
		tok->prev = NULL;
		(*head)->prev = tok;
		*head = tok;
	}
}

void	tk_list_insert(t_tokens *tokens, t_token *after_me, t_token *tok)
{
	trace_info_nvstr(LVL_LEXER, "Token list insert     ", debug_get_token_type(tok->type));
	if (after_me == NULL)
	{
		tk_append(&tokens->head, tok);
	}
	else
	{
		tok->next = after_me->next;
		tok->prev = after_me;
		if (after_me->next)
			after_me->next->prev = tok;
		else
			tokens->tail = tok;
		after_me->next = tok;
	}
	tokens->count++;
}

void	tk_list_append(t_tokens *tokens, t_token *tok)
{
	if (tokens == NULL || tok == NULL)
	{
		trace_info(LVL_LEXER, "Token list append: NULL");
		return ;
	}
	trace_info_nvstr(LVL_LEXER, "Token list append", debug_get_token_type(tok->type));
	tok->next = NULL;
	tok->prev = NULL;
	if (tokens->head == NULL)
	{
		tokens->head = tok;
		tokens->tail = tok;
	}
	else
	{
		tokens->tail->next = tok;
		tok->prev = tokens->tail;
		tokens->tail = tok;
	}
	tokens->count++;
}

/**
 * @brief Remplace un token par une liste de tokens.
 * @param tokens La liste de tokens dans laquelle effectuer le remplacement.
 * @param old Le token à remplacer.
 * @param new_head La tête de la nouvelle liste de tokens.
 * @note Le free de token OLD est géré.
 */
void	replace_in_list(t_tokens *tokens, t_token *old, t_token *new_head)
{
	t_token	*tail;
	t_token	*it;

	debug_pico_title(LVL_LEXER, "Replacing token by list", "\001\033[95m\002");
	tail = new_head;
	while (tail->next)
		tail = tail->next;
	if (old->prev)
		old->prev->next = new_head;
	else
		tokens->head = new_head;
	new_head->prev = old->prev;
	if (old->next)
		old->next->prev = tail;
	tail->next = old->next;
	if (tokens->tail == old)
		tokens->tail = tail;
	tokens->count = 0;
	it = tokens->head;
	while (it)
	{
		tokens->count++;
		it = it->next;
	}
	free_token(old);
}
