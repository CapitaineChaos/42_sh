/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 01:44:48 by smaitre           #+#    #+#             */
/*   Updated: 2026/07/06 00:00:00 by codex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_token.h"
#include "module_lexer.h"
#include "module_input.h"

static bool	match_control(t_lexer *lx, char *str, t_tk_type type)
{
	if (!match_str(&lx->inp, str))
		return (false);
	trace_info(LVL_LEXER, " -- Matching control -- ");
	trace_info_nvstr(LVL_LEXER, " -- ", get_ptr(&lx->inp));
	tk_word_emit(lx);
	tk_control_emit(lx, str, type);
	return (true);
}

static bool	match_operator(t_lexer *lx, char *str, t_tk_type type)
{
	if (!match_str(&lx->inp, str))
		return (false);
	trace_info(LVL_LEXER, " -- Matching operator -- ");
	trace_info_nvstr(LVL_LEXER, " -- ", get_ptr(&lx->inp));
	tk_word_emit(lx);
	tk_operator_emit(lx, str, type);
	return (true);
}

static bool	lex_operator(t_lexer *lx)
{
	if (match_operator(lx, "&&", TOK_AND_IF))
		return (add_context(lx, TOK_AND_IF), true);
	if (match_operator(lx, "||", TOK_OR_IF))
		return (add_context(lx, TOK_OR_IF), true);
	if (match_operator(lx, "|", TOK_PIPE))
		return (add_context(lx, TOK_PIPE), true);
	if (match_control(lx, ";", TOK_SEMI))
		return (true);
	return (false);
}

static bool	match_redir(t_lexer *lx, char *str, t_tk_type type)
{
	if (!match_str(&lx->inp, str))
		return (false);
	rem_op_context(lx);
	tk_word_emit(lx);
	tk_redir_emit(lx, str, type);
	return (true);
}

static bool	lex_redirection(t_lexer *lx)
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

static bool	match_struct(t_lexer *lx, char *str, t_tk_type type)
{
	if (!peek_str_(&lx->inp, str))
		return (false);
	tk_word_emit(lx);
	erase_and_advance_str(&lx->inp, str);
	tk_struct_emit(lx, str, type);
	return (true);
}

static bool	lex_struct(t_lexer *lx)
{
	if (match_struct(lx, "(", TOK_LPAREN))
		return (add_context(lx, TOK_LPAREN), true);
	if (match_struct(lx, ")", TOK_RPAREN))
	{
		rem_sub_context(lx);
		rem_op_context(lx);
		return (true);
	}
	return (false);
}

static void	lex_until_dquotes_end(t_lexer *lx)
{
	while (!is_dquote(lx))
	{
		if (is_backslash(lx) && peek_next_chr(&lx->inp) != '\0')
			advance_(&lx->inp);
		if (is_eof(lx))
			return ;
		advance_(&lx->inp);
	}
	erase_and_advance(&lx->inp);
	remove_matching_context(&lx->ctxs, CTX___DQUOTE);
	lx->pending_dquote = false;
	trace_info(LVL_LEXER, "Ending lex dquote");
}

static bool	lex_dquotes_start(t_lexer *lx)
{
	if (lx->pending_squote || !is_dquote(lx))
		return (false);
	erase_and_advance(&lx->inp);
	trace_info(LVL_LEXER, "Start lex dquote");
	lx->pending_dquote = true;
	rem_op_context(lx);
	add_context(lx, TOK_DQUOTE);
	tk_temp_part_create(lx, TOK_DQUOTE);
	lex_until_dquotes_end(lx);
	return (true);
}

static void	lex_until_squotes_end(t_lexer *lx)
{
	while (!is_squote(lx))
	{
		if (is_eof(lx))
			return ;
		advance_(&lx->inp);
	}
	erase_and_advance(&lx->inp);
	remove_matching_context(&lx->ctxs, CTX___SQUOTE);
	lx->pending_squote = false;
	trace_info(LVL_LEXER, "Ending lex squote");
}

static bool	lex_squotes_start(t_lexer *lx)
{
	if (lx->pending_dquote || !is_squote(lx))
		return (false);
	erase_and_advance(&lx->inp);
	trace_info(LVL_LEXER, "Start lex squote");
	lx->pending_squote = true;
	rem_op_context(lx);
	add_context(lx, TOK_SQUOTE);
	tk_temp_part_create(lx, TOK_SQUOTE);
	lex_until_squotes_end(lx);
	return (true);
}

static bool	handle_backslash(t_lexer *lx)
{
	if (!is_backslash(lx))
		return (false);
	lx->pending_escape = true;
	advance_(&lx->inp);
	if (!is_eof(lx))
	{
		lx->pending_escape = false;
		advance_(&lx->inp);
	}
	return (true);
}

static bool	handle_comment(t_lexer *lx)
{
	if ((lx->pending_dquote || lx->pending_squote) && !lx->pending_escape)
		return (false);
	if (!is_comment(lx))
		return (false);
	tk_word_emit(lx);
	lx->pending_escape = false;
	erase_and_advance(&lx->inp);
	while (!is_eof(lx) && !is_backslash(lx))
		advance_(&lx->inp);
	return (true);
}

static void	lex_until_unquoted_end(t_lexer *lx)
{
	bool	had_char;

	had_char = false;
	while (!is_eof(lx))
	{
		if (lex_operator(lx) || lex_redirection(lx) || lex_struct(lx))
		{
			if (had_char)
				tk_word_emit(lx);
			return ;
		}
		if (is_sep(lx) || is_wsp(lx) || is_dquote(lx) || is_squote(lx))
		{
			trace_info(LVL_LEXER, "End lex unquoted");
			return ;
		}
		if (handle_backslash(lx))
			continue ;
		advance_(&lx->inp);
		had_char = true;
	}
	trace_info(LVL_LEXER, "End lex unquoted");
}

static void	skip_spaces(t_lexer *lx, bool *had_sep)
{
	while (is_wsp(lx))
	{
		if (!*had_sep)
		{
			debug_nano_title(LVL_LEXER, "Word separator");
			tk_word_emit(lx);
			*had_sep = true;
		}
		if (is_newline(lx))
		{
			debug_nano_title(LVL_LEXER, "New line");
			tk_control_emit(lx, "newline", TOK_NEWLINE);
		}
		erase_and_advance(&lx->inp);
	}
}

static void	lex_unquoted(t_lexer *lx)
{
	bool	had_sep;

	if (is_eof(lx))
	{
		lx->pending_escape = false;
		return ;
	}
	had_sep = false;
	skip_spaces(lx, &had_sep);
	if (had_sep || handle_comment(lx))
		return ;
	trace_info(LVL_LEXER, "Start lex unquoted");
	trace_info_nvstr(LVL_LEXER, " -- ", get_ptr(&lx->inp));
	if (!is_eof(lx))
		tk_temp_part_create(lx, TOK_UQUOTE);
	rem_op_context(lx);
	lex_until_unquoted_end(lx);
}

static void	manage_pendings(t_lexer *lx)
{
	debug_lx_pendings(lx);
	rem_esc_context(lx);
	lx->pending_escape = false;
	if (lx->pending_dquote)
		lex_until_dquotes_end(lx);
	else if (lx->pending_squote)
		lex_until_squotes_end(lx);
}

static void	do_lexing(t_lexer *lx)
{
	while (!is_eof(lx))
	{
		if (lex_squotes_start(lx))
			continue ;
		if (lex_dquotes_start(lx))
			continue ;
		if (lex_operator(lx))
			continue ;
		if (lex_redirection(lx))
			continue ;
		if (lex_struct(lx))
			continue ;
		lex_unquoted(lx);
	}
}

void	run_lexer(t_lexer *lx, int lv)
{
	if (lv < 3)
		return ;
	debug_title(LVL_LEXER, "[  Lexing  ]");
	lx->heredoc_count = 0;
	if (is_eof(lx))
		return ;
	manage_pendings(lx);
	do_lexing(lx);
	debug_lx_pendings(lx);
	if (lx->pending_escape)
		add_context(lx, TOK_ESCAPE);
	else if (!lx->pending_squote && !lx->pending_dquote)
	{
		tk_word_emit(lx);
		if (isatty(STDIN_FILENO))
			tk_control_emit(lx, "newline", TOK_NEWLINE);
		if (lx->ctxs.count == 0)
		{
			trace_info(LVL_LEXER, "Context stack is empty");
			tk_control_emit(lx, "eof", TOK_EOF);
		}
		trace_info_nvnb(LVL_LEXER, "Contexts count", lx->ctxs.count);
	}
	trace_info(LVL_LEXER, "[  End lexer  ]");
}
