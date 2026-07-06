/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 23:41:54 by smaitre           #+#    #+#             */
/*   Updated: 2026/07/06 00:00:00 by codex            ###   ########.fr       */
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

static void	free_parser_deques(t_pdeques *deques)
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
	free_parser_deques(&p->deques);
	p->deques.final.head = NULL;
	p->deques.final.tail = NULL;
	p->deques.final.size = 0;
}

void	print_wildcat_error(char *str)
{
	t_logger	lg;

	log_init(&lg);
	log_puts(&lg, "🐯: syntax error near unexpected token `");
	log_puts(&lg, str);
	log_puts(&lg, "'\n");
	log_flush(STDERR_FILENO, &lg, false);
	trace_logger_flush(-1, &lg, true);
}

static bool	is_empty_command(t_ast_node *root)
{
	return (root && root->tclass == ACL_OPERAND && root->type == AST_CMD
		&& root->left == NULL
		&& root->t_ast_data.operand.tokens.count == 0
		&& root->t_ast_data.operand.redirections.count == 0);
}

bool	run_parser(t_parser *prs, t_tokens *tkns, int lv)
{
	t_ast_node	*root;

	if (lv < 4)
		return (false);
	debug_title(LVL_PARSER, "[  Parsing  ]");
	if (tkns->count == 0)
	{
		trace_info(LVL_PARSER, "Aucun token à parser");
		return (false);
	}
	root = rd_parse(tkns);
	if (is_empty_command(root))
		return (free_ast(root), false);
	ps_push_front_create(&prs->deques.final, root);
	debug_ast_tree(&prs->deques.final);
	dbg_nodes(root);
	dbg_ast(&prs->deques.final);
	return (true);
}
