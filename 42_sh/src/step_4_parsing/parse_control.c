/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_control.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 03:42:58 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 21:58:05 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_parser.h"
#include "module_token.h"

bool	parse_control(t_tokens *tokens, t_deque *input)
{
	t_ast_node	*node;
	t_token		*token;
	t_token		*tmp;

	token = tokens->head;
	if (token->family != TKF_CONTROL)
		return (false);
	debug_nano_title(LVL_PARSER, "Parse control");
	trace_info_nvstr(LVL_PARSER, "Token", tokens->head->str);
	token = tk_list_pop_front(tokens);
	if (token->type == TOK_EOF)
		return (free_token(token), true);
	if (token->type == TOK_NEWLINE)
	{
		tmp = tokens->head;
		while (tmp && tmp->type == TOK_NEWLINE)
		{
			tmp = tk_list_pop_front(tokens);
			free_token(tmp);
			tmp = tokens->head;
		}
		if (input->size == 0)
			return (free_token(token), false);
	}
	node = create_control(get_ast_type(token->type), token);
	ps_push_front_create(input, node);
	return (true);
}
