/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lx_tokenize_op_rd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 01:44:48 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 00:05:35 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_token.h"
#include "module_lexer.h"
#include "module_input.h"

static bool	match_control(t_lexer *lx, char *str, t_tk_type t)
{
	if (match_str(&lx->inp, str))
	{
		trace_info(LVL_LEXER, " -- Matching control -- ");
		trace_info_nvstr(LVL_LEXER, " -- ", get_ptr(&lx->inp));
		tk_word_emit(lx);
		tk_control_emit(lx, str, t);
		return (true);
	}
	return (false);
}	

static bool	match_operator(t_lexer *lx, char *str, t_tk_type t)
{
	if (match_str(&lx->inp, str))
	{
		trace_info(LVL_LEXER, " -- Matching operator -- ");
		trace_info_nvstr(LVL_LEXER, " -- ", get_ptr(&lx->inp));
		tk_word_emit(lx);
		tk_operator_emit(lx, str, t);
		return (true);
	}
	return (false);
}

bool	lex_operator(t_lexer *lx)
{
	if (match_operator(lx, "&&", TOK_AND_IF))
	{
		add_context(lx, TOK_AND_IF);
		return (true);
	}
	if (match_operator(lx, "||", TOK_OR_IF))
	{
		add_context(lx, TOK_OR_IF);
		return (true);
	}
	if (match_operator(lx, "|", TOK_PIPE))
	{
		add_context(lx, TOK_PIPE);
		return (true);
	}
	if (match_control(lx, ";", TOK_SEMI))
		return (true);
	return (false);
}

static bool	match_redir(t_lexer *lx, char *str, t_tk_type t)
{
	if (match_str(&lx->inp, str))
	{
		rem_op_context(lx);
		tk_word_emit(lx);
		tk_redir_emit(lx, str, t);
		return (true);
	}
	return (false);
}

bool	lex_redirection(t_lexer *lx)
{
	if (match_redir(lx, ">>", TOK_REDIR_APPEND))
		return (true);
	if (match_redir(lx, "<<", TOK_REDIR_HEREDOC))
		return (true);
	if (match_redir(lx, ">", TOK_REDIR_OUT))
		return (true);
	if (match_redir(lx, "<", TOK_REDIR_IN))
		return (true);
	return (false);
}
