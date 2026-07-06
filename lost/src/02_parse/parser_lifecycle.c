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

#include "module_token.h"
#include "module_parser.h"
#include "ft_std.h"

void	parser_init(t_parser *p, int lv)
{
	if (lv < 4)
		return ;
	p->deques.input.head = NULL;
	p->deques.input.tail = NULL;
	p->deques.input.size = 0;
	p->deques.input.name = "input";
	p->deques.output.head = NULL;
	p->deques.output.tail = NULL;
	p->deques.output.size = 0;
	p->deques.output.name = "output";
	p->deques.final.head = NULL;
	p->deques.final.tail = NULL;
	p->deques.final.size = 0;
	p->deques.final.name = "final";
	p->deques.operators.head = NULL;
	p->deques.operators.tail = NULL;
	p->deques.operators.size = 0;
	p->deques.operators.name = "operators";
}

void	free_pdeques(t_pdeques *deques)
{
	if (deques->input.size > 0)
	{
		deque_free(&deques->input, (void *)free_ast_node);
	}
	if (deques->output.size > 0)
	{
		deque_free(&deques->output, (void *)free_ast_node);
	}
	if (deques->operators.size > 0)
	{
		deque_free(&deques->operators, (void *)free_ast_node);
	}
	if (deques->final.size > 0)
	{
		deque_free(&deques->final, (void *)free_ast);
	}
}

void	parser_free(t_parser *p, int lv)
{
	if (lv < 4)
		return ;
	free_pdeques(&p->deques);
	p->deques.output.head = NULL;
	p->deques.output.tail = NULL;
	p->deques.output.size = 0;
	p->deques.final.head = NULL;
	p->deques.final.tail = NULL;
	p->deques.final.size = 0;
	p->deques.operators.head = NULL;
	p->deques.operators.tail = NULL;
	p->deques.operators.size = 0;
}
