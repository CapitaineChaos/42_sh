/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tk_parts.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 20:07:37 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/25 23:03:14 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_token.h"
#include "module_lexer.h"

/**
 * @brief Crée un subtoken temporaire de type WORDPART
 * @param lx Le lexer
 * @param type Le type du subtoken
 * @note Le subtoken est ajouté à la liste des subtokens temporaires
 */
void	tk_temp_part_create(t_lexer *lx, t_tk_type type)
{
	t_tk_part	*wp;
	t_tk_part	*cur;

	wp = tk_part_new(type, lx->inp.pos);
	if (!wp)
		free_and_exit_minishell(EXIT_FAILURE);
	if (lx->tmp_wp == NULL)
		lx->tmp_wp = wp;
	else
	{
		cur = lx->tmp_wp;
		while (cur->next)
			cur = cur->next;
		cur->next = wp;
		wp->prev = cur;
	}
	trace_info_nvstr(LVL_LEXER, "Temp word part created", debug_get_token_type(wp->type));
}

t_tk_part	*pop_front_part(t_token *token)
{
	t_tk_part	*part;

	if (token == NULL)
		return (NULL);
	if (token->head == NULL)
		return (NULL);
	part = token->head;
	token->head = part->next;
	if (token->head != NULL)
		token->head->prev = NULL;
	else
		token->tail = NULL;
	part->next = NULL;
	part->prev = NULL;
	if (token->count > 0)
		token->count--;
	return (part);
}

void	append_part_to_part(t_tk_part **first, t_tk_part *new)
{
	t_tk_part	*current;

	if (!first || !new)
		return ;
	if (!*first)
	{
		*first = new;
		return ;
	}
	current = *first;
	while (current->next)
		current = current->next;
	current->next = new;
}


void	tk_append_part_tok(t_token *token, t_tk_part *part)
{
	if (!token || !part)
	{
		return ;
	}
	part->next = NULL;
	part->prev = NULL;
	if (token->head == NULL)
	{
		token->head = part;
		token->tail = part;
		token->count = 1;
		return ;
	}
	part->prev = token->tail;
	token->tail->next = part;
	token->tail = part;
	token->count++;
}

t_tk_part	*tk_part_new(t_tk_type type, size_t offset)
{
	t_tk_part	*wp;

	wp = ft_calloc(1, sizeof(*wp));
	if (!wp)
	{
		free_and_exit_minishell(EXIT_FAILURE);
		return (NULL);
	}
	wp->type = type;
	wp->offset = offset;
	return (wp);
}
