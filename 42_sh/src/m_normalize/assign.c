/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 19:18:53 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 19:44:49 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_normalize.h"

void	assign_string(t_token *token)
{
	t_tk_part	*part;

	if (token == NULL)
		return ;
	part = token->head;
	while (part)
	{
		if (part->str == NULL)
			part->str = slice_dup(token->source, part->start, part->end);
		part = part->next;
	}
	token = token->next;
}

void	assign_strings(t_tokens *tokens)
{
	t_token	*token;

	if (tokens == NULL)
		return ;
	token = tokens->head;
	while (token)
	{
		assign_string(token);
		token = token->next;
	}
}

void	assign_parts_source(t_token *token, char *stream)
{
	t_tk_part	*part;

	if (token == NULL)
		return ;
	token->source = stream;
	part = token->head;
	while (part)
	{
		if (part->str == NULL)
			part->str = slice_dup(stream, part->start, part->end);
		part = part->next;
	}
}
