/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_exp.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 02:41:02 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 21:15:07 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_expand.h"
#include "module_assembly.h"
#include "module_normalize.h"
#include "module_debug.h"
#include "module_token.h"
#include "module_ast.h"

int	expand_redirs(t_ast_node *node)
{
	t_operand	*op;
	int			ret;

	op = &node->t_ast_data.operand;
	debug_mini_title(LVL_EXPAND, "Expanding / assembling redirections");
	if (op->redirections.count == 0)
		return (0);
	assign_strings(&op->redirections);
	tag_tokens_tildes(op->redirections.head);
	tag_tokens_vars(op->redirections.head);
	debug_pico_title(LVL_EXPAND, "Expanding tildes and vars", "\033[92m");
	expand_heredoc_vars(&op->redirections);
	expand_redir_tildes(&op->redirections);
	expand_redir_vars(&op->redirections);
	ret = split_tokens(&op->redirections, true);
	if (ret != -1)
		return (return_and_send_error(ret));
	debug_pico_title(LVL_EXPAND, "Expanding wildcards", "\033[92m");
	tag_tokens_globs(op->redirections.head);
	normalize_tokens(&op->redirections);
	aggregate_token_list(op->redirections.head);
	if (expand_redir_wildcards(&op->redirections) == -1)
	{
		debug_mini_title(LVL_EXPAND, "Expanded and assembled redir ended");
		trace_info_nvstr(LVL_EXPAND, "Error", "Failed to expand redirections");
		return (-1);
	}
	debug_mini_title(LVL_EXPAND, "Expanded and assembled redir ended");
	return (0);
}

void	expand_args(t_ast_node *node)
{
	t_operand	*op;

	op = &node->t_ast_data.operand;
	debug_mini_title(LVL_EXPAND, "Expanding / assembling command and args");
	if (op->tokens.count == 0)
		return ;
	assign_strings(&op->tokens);
	tag_tokens_tildes(op->tokens.head);
	tag_tokens_vars(op->tokens.head);
	debug_pico_title(LVL_EXPAND, "Expanding tildes and vars", "\033[92m");
	expand_cmd_tildes(&op->tokens);
	expand_cmd_vars(&op->tokens);
	split_tokens(&op->tokens, false);
	debug_pico_title(LVL_EXPAND, "Expanding wildcards", "\033[92m");
	tag_tokens_globs(op->tokens.head);
	normalize_tokens(&op->tokens);
	aggregate_token_list(op->tokens.head);
	expand_cmd_wildcards(&op->tokens);
	assemble_node(node);
	debug_argv("Command arguments", op->argv);
	debug_mini_title(LVL_EXPAND, "Expanded and assembled command ended");
}
