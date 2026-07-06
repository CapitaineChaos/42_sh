/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:17:32 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 20:10:51 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_parser.h"
#include "module_token.h"

bool	parse_word(t_tokens *tokens, t_deque *input)
{
	t_ast_node	*node;
	t_token		*token;

	token = tokens->head;
	if (!is_operand(token->type))
		return (false);
	if (input->head
		&& (((t_ast_node *)input->head->node)->tclass == ACL_OPERAND))
		node = (t_ast_node *)input->head->node;
	else
	{
		node = create_cmd_word_operand(AST_CMD);
		ps_push_front_create(input, node);
	}
	while (token && is_operand(token->type))
	{
		token = tk_list_pop_front(tokens);
		if (token->group == TKG_REDIRECTION)
			tk_list_append(&node->t_ast_data.operand.redirections, token);
		else
			tk_list_append(&node->t_ast_data.operand.tokens, token);
		token = tokens->head;
	}
	return (true);
}
