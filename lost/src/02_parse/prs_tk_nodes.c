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

#include "module_token.h"
#include "module_parser.h"

void	find_groups(t_tokens *tokens)
{
	t_token	*tok;

	tok = tokens->head;
	while (tok)
	{
		if (is_subshell(tok->type) && tok->type == TOK_RPAREN)
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
		if (is_redirect(tok->type))
		{
			tok->group = TKG_REDIRECTION;
			if (tok->next && tok->type == TOK_REDIR_HEREDOC)
			{
				tok->next->group = TKG_REDIRECTION;
				tok->next->type = TOK_DELIM;
				tok = tok->next;
				if (tok->next && is_heredoc_body(tok->next->type))
					tok->next->group = TKG_REDIRECTION;
			}
			else if (tok->next && tok->next->type == TOK_WORD)
				tok->next->group = TKG_REDIRECTION;
		}
		tok = tok->next;
	}
}

void	tokens_to_nodes(t_tokens *tokens, t_deque *input)
{
	t_token	*tok;

	find_redirs(tokens);
	find_groups(tokens);
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
		}
	}
}
