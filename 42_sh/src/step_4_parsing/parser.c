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
}

static void	free_parser_deques(t_pdeques *deques)
{
	if (deques->final.size > 0)
	{
		deque_free(&deques->final, (void *)free_ast);
	}
}

void	parser_free(t_parser *p, int lv)
{
	if (lv < 4)
		return ;
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
	t_dq_n		*cell;

	if (lv < 4)
		return (false);
	if (tkns->count == 0)
	{
		return (false);
	}
	root = rd_parse(tkns);
	if (is_empty_command(root))
		return (free_ast(root), false);
	if (root)
	{
		cell = dstn_new(root);
		if (!cell)
			free_and_exit_minishell(EXIT_FAILURE);
		deque_push_front(&prs->deques.final, cell);
	}
	dbg_nodes(root);
	dbg_ast(&prs->deques.final);
	return (true);
}
