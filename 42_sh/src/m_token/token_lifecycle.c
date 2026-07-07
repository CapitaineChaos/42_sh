/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lifecycle.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 01:35:59 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/31 23:08:42 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_token.h"
#include "all_config.h"
#include "module_minishell.h"
#include <stdlib.h>

t_token	*token_new(t_tk_type type)
{
	t_token	*tok;

	tok = calloc(1, sizeof(*tok));
	if (!tok)
	{
		free_and_exit_minishell(EXIT_FAILURE);
		return (NULL);
	}
	tok->type = type;
	return (tok);
}

void	free_token_part(t_tk_part *part)
{
	if (part == NULL)
		return ;
	if (part->str != NULL)
	{
		free(part->str);
		part->str = NULL;
	}
	free(part);
	part = NULL;
}

void	free_token_parts(t_tk_part *first_part)
{
	t_tk_part	*next;
	int			i;

	i = 0;
	while (first_part)
	{
		next = first_part->next;
		free_token_part(first_part);
		first_part = next;
		i++;
	}
}

/**
 * @brief Simple free de token.
 * @param token Le token à libérer.
 * @note Si le token est de type TOK_WORD, il libère également les parties
 *       internes du token.
 */
void	free_token(t_token *token)
{
	if (!token)
		return ;
	if (token->str != NULL)
	{
		free(token->str);
		token->str = NULL;
	}
	if (token->type == TOK_WORD || token->type == TOK_DELIM
		|| token->type == TOK_HEREDOC_BODY
		|| token->type == TOK_GLOB)
	{
		free_token_parts(token->head);
		token->head = NULL;
		token->tail = NULL;
		token->count = 0;
	}
	free(token);
}

void	free_token_list(t_tokens *tokens)
{
	t_token	*cur;
	t_token	*next;
	int		i;

	if (!tokens)
		return ;
	i = 0;
	cur = tokens->head;
	while (cur)
	{
		next = cur->next;
		free_token(cur);
		cur = next;
		i++;
	}
	tokens->head = NULL;
	tokens->tail = NULL;
	tokens->count = 0;
}
