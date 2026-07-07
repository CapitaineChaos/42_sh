/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rd_parser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 00:00:00 by smaitre           #+#    #+#             */
/*   Updated: 2025/07/06 00:00:00 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_parser.h"
#include "module_token.h"

static t_ast_node	*rd_list(t_tokens *t);

void	find_redirs(t_tokens *tokens)
{
	t_token	*tok;

	tok = tokens->head;
	while (tok)
	{
		if (tok_has(tok->type, TF_REDIR))
		{
			tok->role = TKR_REDIR_OP;
			if (tok->next && tok->type == TOK_REDIR_HEREDOC)
			{
				tok->next->role = TKR_HEREDOC_DELIM;
				tok = tok->next;
				if (tok->next && tok->next->type == TOK_HEREDOC_BODY)
					tok->next->role = TKR_HEREDOC_BODY;
			}
			else if (tok->next && tok->next->type == TOK_WORD)
				tok->next->role = TKR_REDIR_TARGET;
		}
		tok = tok->next;
	}
}

static bool	is_type(t_tokens *t, t_tk_type ty)
{
	return (t->head && t->head->type == ty);
}

static bool	is_seq_sep(t_token *tok)
{
	return (tok && tok_has(tok->type, TF_LIST_SEP));
}

static bool	starts_command(t_token *tok)
{
	return (tok && (tok_has(tok->type, TF_OPERAND)
			|| tok->type == TOK_LPAREN));
}

static bool	is_redir_role(t_token *tok)
{
	return (tok && (tok->role == TKR_REDIR_OP
			|| tok->role == TKR_REDIR_TARGET
			|| tok->role == TKR_HEREDOC_DELIM
			|| tok->role == TKR_HEREDOC_BODY));
}

static t_ast_node	*bin_node(t_ast_type type, t_token *op,
	t_ast_node *left, t_ast_node *right)
{
	t_ast_node	*node;

	if (type == AST_SEQ)
		node = create_binary(type, op, ACL_CONTROL);
	else
		node = create_binary(type, op, ACL_OPERATOR);
	node->left = left;
	node->right = right;
	return (node);
}

static void	consume_operands(t_tokens *t, t_ast_node *node, bool redirs_only)
{
	t_token	*tok;

	while (t->head && tok_has(t->head->type, TF_OPERAND))
	{
		if (redirs_only && !is_redir_role(t->head))
			break ;
		tok = tk_list_pop_front(t);
		if (is_redir_role(tok))
			tk_list_append(&node->t_ast_data.operand.redirections, tok);
		else
			tk_list_append(&node->t_ast_data.operand.tokens, tok);
	}
}

static t_ast_node	*rd_simple_command(t_tokens *t)
{
	t_ast_node	*node;

	node = create_cmd_word_operand(AST_CMD);
	consume_operands(t, node, false);
	return (node);
}

static t_ast_node	*rd_subshell(t_tokens *t)
{
	t_ast_node	*node;

	free_token(tk_list_pop_front(t));
	node = create_cmd_word_operand(AST_SUBSHELL);
	node->left = rd_list(t);
	if (is_type(t, TOK_RPAREN))
		free_token(tk_list_pop_front(t));
	consume_operands(t, node, true);
	return (node);
}

static t_ast_node	*rd_command(t_tokens *t)
{
	if (is_type(t, TOK_LPAREN))
		return (rd_subshell(t));
	return (rd_simple_command(t));
}

static t_ast_node	*rd_pipeline(t_tokens *t)
{
	t_ast_node	*left;
	t_token		*op;

	left = rd_command(t);
	while (is_type(t, TOK_PIPE))
	{
		op = tk_list_pop_front(t);
		left = bin_node(AST_PIPE, op, left, rd_command(t));
	}
	return (left);
}

static t_ast_node	*rd_and_or(t_tokens *t)
{
	t_ast_node	*left;
	t_token		*op;

	left = rd_pipeline(t);
	while (t->head && tok_has(t->head->type, TF_AND_OR_OP))
	{
		op = tk_list_pop_front(t);
		left = bin_node(get_ast_type(op->type), op, left, rd_pipeline(t));
	}
	return (left);
}

static t_ast_node	*rd_list(t_tokens *t)
{
	t_ast_node	*left;
	t_token		*sep;

	left = rd_and_or(t);
	while (is_seq_sep(t->head))
	{
		sep = tk_list_pop_front(t);
		if (starts_command(t->head))
			left = bin_node(AST_SEQ, sep, left, rd_and_or(t));
		else
			free_token(sep);
	}
	return (left);
}

static void	assign_ids(t_ast_node *node, t_ast_node *parent, int *id)
{
	if (!node)
		return ;
	node->parent = parent;
	node->id = (*id)++;
	assign_ids(node->left, node, id);
	assign_ids(node->right, node, id);
}

t_ast_node	*rd_parse(t_tokens *tokens)
{
	t_ast_node	*root;
	int			id;

	find_redirs(tokens);
	root = rd_list(tokens);
	while (tokens->head)
		free_token(tk_list_pop_front(tokens));
	id = 0;
	assign_ids(root, NULL, &id);
	return (root);
}
