/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tk_op_redir_emit.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 19:45:17 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 03:23:10 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_token.h"
#include "module_read.h"
#include "utils.h"

void	tk_heredoc_body_emit(t_lexer *lx, t_token *next, char *body)
{
	t_token	*token;

	trace_start(LVL_LEXER, "Emit heredoc body token");
	token = token_new(TOK_HEREDOC_BODY);
	if (!token)
	{
		free_and_exit_minishell(EXIT_FAILURE);
		return ;
	}
	token->kind = TKD_OPERAND;
	token->family = TKF_HEREDOC_BODY;
	token->group = TKG_OTHER;
	if (body)
		token->str = ft_strdup(body);
	tk_list_insert(&lx->tokens, next, token);
	trace_info_nvstr(LVL_LEXER, "Token heredoc body emitted", debug_get_token_type(token->type));
}

void	tk_struct_emit(t_lexer *lx, char *s, t_tk_type type)
{
	t_token	*token;

	trace_start(LVL_LEXER, "Emit struct token");
	token = token_new(type);
	if (!token)
	{
		free_and_exit_minishell(EXIT_FAILURE);
		return ;
	}
	token->kind = TKD_STRUCT;
	token->family = TKF_SUBSHELL;
	token->group = TKG_OTHER;
	token->str = ft_strdup(s);
	tk_list_append(&lx->tokens, token);
	trace_info_nvstr(LVL_LEXER, "Token struct emitted", debug_get_token_type(token->type));
}

void	tk_redir_emit(t_lexer *lx, char *str, t_tk_type type)
{
	t_token	*token;

	trace_start(LVL_LEXER, "Emit redir token");
	token = token_new(type);
	if (!token)
	{
		free_and_exit_minishell(EXIT_FAILURE);
		return ;
	}
	token->kind = TKD_OPERAND;
	token->family = TKF_REDIRECT;
	token->group = TKG_OTHER;
	token->str = ft_strdup(str);
	if (type == TOK_REDIR_HEREDOC)
	{
		token->tk_flag = true;
		lx->heredoc_count++;
	}
	else
		token->tk_flag = false;
	tk_list_append(&lx->tokens, token);
	trace_info_nvstr(LVL_LEXER, "Token Redirect emitted", debug_get_token_type(token->type));
}
