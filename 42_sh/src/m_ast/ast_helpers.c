/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 21:12:39 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/12 16:48:55 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_ast.h"
#include "ft_std.h"

t_ast_node	*peek_head(t_deque *deque)
{
	t_ast_node	*node;

	if (deque->size == 0)
		return (NULL);
	node = (t_ast_node *)deque->head->node;
	return (node);
}
