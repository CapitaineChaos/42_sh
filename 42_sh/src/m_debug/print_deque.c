/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_deque.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 23:19:59 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 20:37:17 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_parser.h"
#include "module_debug.h"
#include "ft_deque.h"
#include <stdio.h>

void	debug_check_integrity(t_parser *prs)
{
	debug_mini_title(LVL_PARSER, "Integrity check");
	if (prs->deques.operators.size > 0)
		trace_info(LVL_PARSER, "Erreur de parsing : il reste des opérateurs non traités");
	if (prs->deques.final.size == 0)
		trace_info(LVL_PARSER, "Erreur de parsing : l'arbre AST est vide");
	if (prs->deques.final.size > 1)
		trace_info(LVL_PARSER, "Erreur de parsing : l'arbre AST contient plusieurs nœuds");
	if (prs->deques.output.size > 0)
		trace_info(LVL_PARSER, "Erreur de parsing : il reste des opérandes non traitées");
}

void	print_content_or_type(t_token *tok)
{
	t_tk_part	*p;
	t_logger	lg;

	log_init(&lg);
	log_puts(&lg, debug_get_token_type(tok->type));
	p = tok->head;
	if (tok->family == TKF_WORD || tok->family == TKF_WORD_REDIR)
	{
		while (p)
		{
			if (p->stream)
			{
				log_puts(&lg, p->stream);
				log_puts(&lg, "\n");
			}
			p = p->next;
		}
	}
	else if (tok->type == TOK_REDIR_APPEND)
		log_puts(&lg, ">>");
	else if (tok->type == TOK_REDIR_IN)
		log_puts(&lg, "<");
	else if (tok->type == TOK_REDIR_OUT)
		log_puts(&lg, ">");
	else if (tok->type == TOK_REDIR_HEREDOC)
		log_puts(&lg, "<<");
	else if (tok->family == TKF_HEREDOC_BODY)
		log_puts(&lg, "HD_BODY");
	log_puts(&lg, "\n");
	log_flush(-1, &lg, true);
}

void	print_ast_operand_tokens(t_ast_node *node)
{
	t_tokens		*toks;
	t_token			*tok;
	t_logger		lg;

	toks = &node->t_ast_data.operand.tokens;
	tok = toks->head;
	while (tok)
	{
		log_init(&lg);
		log_puts(&lg, "  ");
		log_flush(-1, &lg, true);
		print_content_or_type(tok);
		tok = tok->next;
	}
}

void	print_deque_real(t_deque *deque, char *name)
{
	t_dq_n		*current;
	t_ast_node	*node;
	char		buff[1024];

	debug_mini_title(LVL_PARSER, "Printing nodes");
	sprintf(buff, "Deque %s (size: %zu)", name, deque->size); 
	debug_log(buff);
	debug_nano_title(LVL_PARSER, "Nodes");
	current = deque->head;
	while (current)
	{
		node = (t_ast_node *)current->node;
		if (node->tclass == ACL_OPERATOR)
		{
			sprintf(buff, "Operator %15s,    Op    : %14s           P : %3lu\n", get_ast_typestr(node->type), node->t_ast_data.operator_.token->str, node->t_ast_data.operator_.precedence);
			debug_log(buff);
		}
		if (node->tclass == ACL_CONTROL)
		{
			sprintf(buff, "Control %15s,    Ctrl  : %14s           P : %3lu\n", get_ast_typestr(node->type), node->t_ast_data.operator_.token->str, node->t_ast_data.operator_.precedence);
			debug_log(buff);
		}
		if (node->tclass == ACL_STRUCT)
		{
			sprintf(buff, "Struct %15s,    Struct : %14s\n", get_ast_typestr(node->type), node->t_ast_data.operator_.token->str);
			debug_log(buff);
		}
		if (node->tclass == ACL_OPERAND)
		{
			sprintf(buff, "Operand  %15s,    Tk redirs : %3u, Tk words : %3u\n", get_ast_typestr(node->type), node->t_ast_data.operand.redirections.count, node->t_ast_data.operand.tokens.count);
			debug_log(buff);
		}
		current = current->next;
	}
	debug_mini_title(LVL_PARSER, "End of nodes");
}

void	debug_deque(t_deque *deque, char *name)
{
	t_logger	lg;
	if (DLVL >= LVL_PARSER)
	{
		if (deque->size == 0)
		{
			log_init(&lg);
			log_puts(&lg, "\nDeque ");
			log_puts(&lg, name);
			log_puts(&lg, " is empty\n");
			log_flush(-1, &lg, true);
			return ;
		}
		print_deque_real(deque, name);
	}
}
