/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_lifecycle.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:01:42 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/23 03:18:47 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_token.h"
#include "module_parser.h"
#include "ft_std.h"

void	parser_init(t_parser *p, int lv)
{
	if (lv < 4)
		return ;
	p->deques.final.head = NULL;
	p->deques.final.tail = NULL;
	p->deques.final.size = 0;
	p->deques.final.name = "final";
	trace_ok(LVL_PARSER, "Parser init done");
}

void	free_pdeques(t_pdeques *deques)
{
	trace_info(LVL_PARSER, ">> Freeing pstacks");
	if (deques->final.size > 0)
	{
		trace_info(LVL_PARSER, "Freeing " "[   final   ]" " stack");
		deque_free(&deques->final, (void *)free_ast);
	}
	trace_info(LVL_PARSER, "Freeing pstacks done");
}

void	parser_free(t_parser *p, int lv)
{
	if (lv < 4)
		return ;
	trace_info(LVL_PARSER, "Freeing parser");
	free_pdeques(&p->deques);
	p->deques.final.head = NULL;
	p->deques.final.tail = NULL;
	p->deques.final.size = 0;
}
