/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 00:57:34 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/23 02:58:36 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_context.h"

bool	ctx_is_now(t_contexts *ctxs, t_ctx_ ctx_name)
{
	t_context	*ctx_node;

	ctx_node = ctxs->tail;
	if (!ctx_node)
		return (false);
	if (ctx_node->ctx_name == ctx_name)
		return (true);
	return (false);
}

bool	is_hidden_context(t_ctx_ ctx_name)
{
	if (ctx_name == CTX___NONE)
		return (true);
	return (false);
}
