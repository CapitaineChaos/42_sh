/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_tk_nodes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 19:07:36 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 04:47:29 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_token.h"
#include "module_parser.h"

void	find_groups(t_tokens *tokens)
{
	t_token	*tok;

	tok = tokens->head;
	while (tok)
	{
		if (tok->kind == TKD_STRUCT && tok->type == TOK_RPAREN)
		{
			tok->group = TKG_GROUP;
		}
		tok = tok->next;	
	}
}

void	find_redirs(t_tokens *tokens)
{
	t_token	*tok;

	tok = tokens->head;
	while (tok)
	{
		if (tok->family == TKF_REDIRECT)
		{
			tok->group = TKG_REDIRECTION;
			if (tok->next && tok->type == TOK_REDIR_HEREDOC)
			{
				tok->next->group = TKG_REDIRECTION;
				tok->next->type = TOK_DELIM;
				tok = tok->next;
				if (tok->next && tok->next->family == TKF_HEREDOC_BODY)
					tok->next->group = TKG_REDIRECTION;
			}
			else if (tok->next && tok->next->type == TOK_WORD)
				tok->next->group = TKG_REDIRECTION;
		}
		tok = tok->next;
	}
}

/**
 * @brief Transforme la liste de tokens en une liste nœuds.
 * @param tokens La liste de tokens à transformer.
 * @param input La pile contenant les nœuds à traiter par la suite.
 * @note La fonction essaie de créer des nœuds de commande ou d'opérateur
 * 	 à partir des tokens. Si un token ne peut pas être transformé, il est
 * 	 libéré et un message d'erreur est affiché.
 */
void	tokens_to_nodes(t_tokens *tokens, t_deque *input)
{
	t_token	*tok;

	debug_title(LVL_PARSER, "[  Transformation des tokens en nœuds  ]");
	find_redirs(tokens);
	find_groups(tokens);
	trace_info_nvnb(LVL_PARSER, "Nb tokens", tokens->count);
	deque_init(input);
	while (tokens->head)
	{
		if (parse_word(tokens, input))
			continue ;
		if (parse_operator(tokens, input))
			continue ;
		if (parse_struct(tokens, input))
			continue ;
		if (parse_control(tokens, input))
			continue ;
		else if (tokens->count > 0)
		{
			tok = tk_list_pop_front(tokens);
			free_token(tok);
			trace_info(LVL_PARSER, "Token non géré");
		}
	}
}
