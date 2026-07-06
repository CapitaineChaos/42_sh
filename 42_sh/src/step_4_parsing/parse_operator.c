/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:19:18 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/25 03:23:14 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_parser.h"
#include "module_token.h"

bool	parse_operator(t_tokens *tokens, t_deque *input)
{
	t_ast_node	*node;
	t_token		*token;

	token = tokens->head;
	if (token->family != TKF_OPERATOR)
		return (false);
	debug_nano_title(LVL_PARSER, "Parse operator");
	trace_info_nvstr(LVL_PARSER, "Token", tokens->head->str);
	token = tk_list_pop_front(tokens);
	node = create_operator(get_ast_type(token->type), token);
	ps_push_front_create(input, node);
	return (true);
}
