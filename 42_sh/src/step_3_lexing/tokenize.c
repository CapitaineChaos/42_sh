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

static bool	emit_control_if_present(t_lexer *lx, char *str, t_tk_type type)
{
	if (!match_str(&lx->inp, str))
		return (false);
	tk_word_emit(lx);
	tk_control_emit(lx, str, type);
	return (true);
}

static bool	emit_operator_if_present(t_lexer *lx, char *str, t_tk_type type)
{
	if (!match_str(&lx->inp, str))
		return (false);
	tk_word_emit(lx);
	tk_operator_emit(lx, str, type);
	return (true);
}

static bool	emit_control_operator(t_lexer *lx)
{
	if (emit_operator_if_present(lx, "&&", TOK_AND_IF))
		return (add_context(lx, TOK_AND_IF), true);
	if (emit_operator_if_present(lx, "||", TOK_OR_IF))
		return (add_context(lx, TOK_OR_IF), true);
	if (emit_operator_if_present(lx, "|", TOK_PIPE))
		return (add_context(lx, TOK_PIPE), true);
	if (emit_control_if_present(lx, ";", TOK_SEMI))
		return (true);
	return (false);
}

static bool	emit_redirection_if_present(t_lexer *lx, char *str, t_tk_type type)
{
	if (!match_str(&lx->inp, str))
		return (false);
	rem_op_context(lx);
	tk_word_emit(lx);
	tk_redir_emit(lx, str, type);
	return (true);
}

static bool	emit_redirection(t_lexer *lx)
{
	if (emit_redirection_if_present(lx, ">>", TOK_REDIR_APPEND))
		return (true);
	if (emit_redirection_if_present(lx, "<<", TOK_REDIR_HEREDOC))
		return (true);
	if (emit_redirection_if_present(lx, ">", TOK_REDIR_OUT))
		return (true);
	if (emit_redirection_if_present(lx, "<", TOK_REDIR_IN))
		return (true);
	return (false);
}

static bool	emit_struct_if_present(t_lexer *lx, char *str, t_tk_type type)
{
	if (!peek_str_(&lx->inp, str))
		return (false);
	tk_word_emit(lx);
	erase_and_advance_str(&lx->inp, str);
	tk_struct_emit(lx, str, type);
	return (true);
}

static bool	emit_struct(t_lexer *lx)
{
	if (emit_struct_if_present(lx, "(", TOK_LPAREN))
		return (add_context(lx, TOK_LPAREN), true);
	if (emit_struct_if_present(lx, ")", TOK_RPAREN))
	{
		rem_sub_context(lx);
		rem_op_context(lx);
		return (true);
	}
	return (false);
}

static void	read_double_quoted_body(t_lexer *lx)
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
}

static bool	read_double_quoted_part(t_lexer *lx)
{
	if (lx->pending_squote || !is_dquote(lx))
		return (false);
	erase_and_advance(&lx->inp);
	lx->pending_dquote = true;
	rem_op_context(lx);
	add_context(lx, TOK_DQUOTE);
	tk_temp_part_create(lx, TOK_DQUOTE);
	read_double_quoted_body(lx);
	return (true);
}

static void	read_single_quoted_body(t_lexer *lx)
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
}

static bool	read_single_quoted_part(t_lexer *lx)
{
	if (lx->pending_dquote || !is_squote(lx))
		return (false);
	erase_and_advance(&lx->inp);
	lx->pending_squote = true;
	rem_op_context(lx);
	add_context(lx, TOK_SQUOTE);
	tk_temp_part_create(lx, TOK_SQUOTE);
	read_single_quoted_body(lx);
	return (true);
}

static bool	consume_escape(t_lexer *lx)
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

static bool	consume_comment(t_lexer *lx)
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

static void	read_unquoted_body(t_lexer *lx)
{
	bool	had_char;

	had_char = false;
	while (!is_eof(lx))
	{
		if (emit_control_operator(lx) || emit_redirection(lx)
			|| emit_struct(lx))
		{
			if (had_char)
				tk_word_emit(lx);
			return ;
		}
		if (is_sep(lx) || is_wsp(lx) || is_dquote(lx) || is_squote(lx))
		{
			return ;
		}
		if (consume_escape(lx))
			continue ;
		advance_(&lx->inp);
		had_char = true;
	}
}

static void	consume_separators(t_lexer *lx, bool *had_sep)
{
	while (is_wsp(lx))
	{
		if (!*had_sep)
		{
			tk_word_emit(lx);
			*had_sep = true;
		}
		if (is_newline(lx))
		{
			tk_control_emit(lx, "newline", TOK_NEWLINE);
		}
		erase_and_advance(&lx->inp);
	}
}

static void	read_unquoted_part(t_lexer *lx)
{
	bool	had_sep;

	if (is_eof(lx))
	{
		lx->pending_escape = false;
		return ;
	}
	had_sep = false;
	consume_separators(lx, &had_sep);
	if (had_sep || consume_comment(lx))
		return ;
	if (!is_eof(lx))
		tk_temp_part_create(lx, TOK_UQUOTE);
	rem_op_context(lx);
	read_unquoted_body(lx);
}

static void	resume_pending_quote(t_lexer *lx)
{
	rem_esc_context(lx);
	lx->pending_escape = false;
	if (lx->pending_dquote)
		read_double_quoted_body(lx);
	else if (lx->pending_squote)
		read_single_quoted_body(lx);
}

static void	tokenize_input(t_lexer *lx)
{
	while (!is_eof(lx))
	{
		if (read_single_quoted_part(lx))
			continue ;
		if (read_double_quoted_part(lx))
			continue ;
		if (emit_control_operator(lx))
			continue ;
		if (emit_redirection(lx))
			continue ;
		if (emit_struct(lx))
			continue ;
		read_unquoted_part(lx);
	}
}

void	run_lexer(t_lexer *lx, int lv)
{
	if (lv < 3)
		return ;
	lx->heredoc_count = 0;
	if (is_eof(lx))
		return ;
	resume_pending_quote(lx);
	tokenize_input(lx);
	if (lx->pending_escape)
		add_context(lx, TOK_ESCAPE);
	else if (!lx->pending_squote && !lx->pending_dquote)
	{
		tk_word_emit(lx);
		if (isatty(STDIN_FILENO))
			tk_control_emit(lx, "newline", TOK_NEWLINE);
		if (lx->ctxs.count == 0)
		{
			tk_control_emit(lx, "eof", TOK_EOF);
		}
	}
}
