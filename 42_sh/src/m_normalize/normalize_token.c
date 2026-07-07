/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_token.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:20:05 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 19:11:58 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_normalize.h"

void	normalize_token(t_token *token)
{
	if (token == NULL)
		return ;
	while (token)
	{
		if (token->family == TKF_WORD)
			normalize_parts(token->head);
		token = token->next;
	}
}

void	normalize_tokens(t_tokens *tokens)
{
	t_token	*token;

	if (tokens->head == NULL)
		return ;
	token = tokens->head;
	normalize_token(token);
}
