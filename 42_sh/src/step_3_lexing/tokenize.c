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

static void	close_tmp_part(t_lexer *lx)
{
	t_tk_part	*part;

	part = lx->tmp_wp;
	if (part == NULL)
		return ;
	while (part->next)
		part = part->next;
	part->end = lx->inp.pos;
}

static void	drop_empty_tmp_uquote(t_lexer *lx)
{
	t_tk_part	*part;

	part = lx->tmp_wp;
	if (part == NULL)
		return ;
	while (part->next)
		part = part->next;
	if (part->type != PART_UQUOTE || part->start != part->end)
		return ;
	if (part->prev)
		part->prev->next = NULL;
	else
		lx->tmp_wp = NULL;
	free_token_part(part);
}

static bool	starts_word_char(t_lexer *lx)
{
	return (!is_eof(lx) && !is_sep(lx) && !is_wsp(lx)
		&& !is_dquote(lx) && !is_squote(lx)
		&& !peek_str_(&lx->inp, "&&") && !peek_str_(&lx->inp, "||")
		&& !peek_str_(&lx->inp, "|") && !peek_str_(&lx->inp, ">>")
		&& !peek_str_(&lx->inp, "<<") && !peek_str_(&lx->inp, ">")
		&& !peek_str_(&lx->inp, "<") && !peek_str_(&lx->inp, "(")
		&& !peek_str_(&lx->inp, ")"));
}

static void	enter_quote_state(t_lexer *lx, t_lex_state state,
	t_tk_part_type part_type)
{
	lx->state = state;
	rem_op_context(lx);
	if (part_type == PART_DQUOTE)
		context_add_(&lx->ctxs, CTX___DQUOTE);
	else if (part_type == PART_SQUOTE)
		context_add_(&lx->ctxs, CTX___SQUOTE);
	tk_temp_part_create(lx, part_type);
}

static void	leave_quote_state(t_lexer *lx, t_ctx_ ctx)
{
	close_tmp_part(lx);
	advance_(&lx->inp);
	remove_matching_context(&lx->ctxs, ctx);
	lx->state = LEX_NORMAL;
}

static bool	emit_control_if_present(t_lexer *lx, char *str, t_tk_type type)
{
	if (!peek_str_(&lx->inp, str))
		return (false);
	close_tmp_part(lx);
	check_str(&lx->inp, str);
	tk_word_emit(lx);
	tk_control_emit(lx, str, type);
	return (true);
}

static bool	emit_operator_if_present(t_lexer *lx, char *str, t_tk_type type)
{
	if (!peek_str_(&lx->inp, str))
		return (false);
	close_tmp_part(lx);
	check_str(&lx->inp, str);
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
	if (!peek_str_(&lx->inp, str))
		return (false);
	rem_op_context(lx);
	close_tmp_part(lx);
	check_str(&lx->inp, str);
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
	close_tmp_part(lx);
	tk_word_emit(lx);
	check_str(&lx->inp, str);
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

static void	lex_dquote(t_lexer *lx)
{
	while (!is_dquote(lx))
	{
		if (is_backslash(lx) && peek_next_chr(&lx->inp) != '\0')
			advance_(&lx->inp);
		if (is_eof(lx))
			return ;
		advance_(&lx->inp);
	}
	leave_quote_state(lx, CTX___DQUOTE);
}

static bool	enter_dquote(t_lexer *lx)
{
	if (!is_dquote(lx))
		return (false);
	advance_(&lx->inp);
	enter_quote_state(lx, LEX_DQUOTE, PART_DQUOTE);
	return (true);
}

static void	lex_squote(t_lexer *lx)
{
	while (!is_squote(lx))
	{
		if (is_eof(lx))
			return ;
		advance_(&lx->inp);
	}
	leave_quote_state(lx, CTX___SQUOTE);
}

static bool	enter_squote(t_lexer *lx)
{
	if (!is_squote(lx))
		return (false);
	advance_(&lx->inp);
	enter_quote_state(lx, LEX_SQUOTE, PART_SQUOTE);
	return (true);
}

static bool	consume_escape(t_lexer *lx)
{
	if (!is_backslash(lx))
		return (false);
	if (peek_next_chr(&lx->inp) == '\n')
	{
		close_tmp_part(lx);
		drop_empty_tmp_uquote(lx);
		advance_(&lx->inp);
		advance_(&lx->inp);
		if (starts_word_char(lx))
			tk_temp_part_create(lx, PART_UQUOTE);
		return (true);
	}
	lx->state = LEX_ESCAPE;
	advance_(&lx->inp);
	if (!is_eof(lx))
	{
		lx->state = LEX_NORMAL;
		advance_(&lx->inp);
	}
	return (true);
}

static bool	consume_comment(t_lexer *lx)
{
	if (lx->state != LEX_NORMAL)
		return (false);
	if (!is_comment(lx))
		return (false);
	close_tmp_part(lx);
	tk_word_emit(lx);
	advance_(&lx->inp);
	while (!is_eof(lx) && !is_newline(lx))
		advance_(&lx->inp);
	return (true);
}

static void	read_unquoted_body(t_lexer *lx)
{
	bool	had_char;

	had_char = false;
	while (!is_eof(lx))
	{
		if (emit_control_operator(lx) || emit_redirection(lx) || emit_struct(lx))
		{
			if (had_char)
				tk_word_emit(lx);
			return ;
		}
		if (is_sep(lx) || is_wsp(lx) || is_dquote(lx) || is_squote(lx))
		{
			close_tmp_part(lx);
			return ;
		}
		if (consume_escape(lx))
			continue ;
		advance_(&lx->inp);
		had_char = true;
	}
	close_tmp_part(lx);
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
		advance_(&lx->inp);
	}
}

static void	lex_escape(t_lexer *lx)
{
	rem_esc_context(lx);
	lx->state = LEX_NORMAL;
}

static void	lex_normal(t_lexer *lx)
{
	bool	had_sep;

	if (enter_squote(lx) || enter_dquote(lx))
		return ;
	if (emit_control_operator(lx) || emit_redirection(lx) || emit_struct(lx))
		return ;
	had_sep = false;
	consume_separators(lx, &had_sep);
	if (had_sep || consume_comment(lx))
		return ;
	if (!is_eof(lx))
		tk_temp_part_create(lx, PART_UQUOTE);
	rem_op_context(lx);
	read_unquoted_body(lx);
}

static void	tokenize_input(t_lexer *lx)
{
	while (!is_eof(lx))
	{
		if (lx->state == LEX_SQUOTE)
			lex_squote(lx);
		else if (lx->state == LEX_DQUOTE)
			lex_dquote(lx);
		else if (lx->state == LEX_ESCAPE)
			lex_escape(lx);
		else
			lex_normal(lx);
	}
}

void	run_lexer(t_lexer *lx)
{
	lx->heredoc_count = 0;
	if (is_eof(lx))
		return ;
	tokenize_input(lx);
	if (lx->state == LEX_ESCAPE)
		add_context(lx, TOK_ESCAPE);
	else if (lx->state == LEX_NORMAL)
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
