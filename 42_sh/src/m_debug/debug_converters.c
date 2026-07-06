/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_converters.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 21:47:28 by smaitre           #+#    #+#             */
/*   Updated: 2026/07/06 00:00:00 by codex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"

typedef struct s_debug_name
{
	int		value;
	char	*name;
}	t_debug_name;

static char	*lookup_name(const t_debug_name *names, int value, char *fallback)
{
	while (names->name)
	{
		if (names->value == value)
			return (names->name);
		names++;
	}
	return (fallback);
}

char	*debug_get_token_type(t_tk_type type)
{
	static const t_debug_name	names[] = {
	{TOK_UQUOTE, "UNQUOTED"},
	{TOK_DQUOTE, "DQUOTE"},
	{TOK_SQUOTE, "SQUOTE"},
	{TOK_GLOB, "GLOB"},
	{TOK_PIPE, "PIPE"},
	{TOK_AND_IF, "AND_IF"},
	{TOK_OR_IF, "OR_IF"},
	{TOK_LPAREN, "LPAREN"},
	{TOK_RPAREN, "RPAREN"},
	{TOK_SEMI, "SEMI"},
	{TOK_REDIR_IN, "REDIR_IN"},
	{TOK_REDIR_OUT, "REDIR_OUT"},
	{TOK_REDIR_APPEND, "REDIR_APPEND"},
	{TOK_REDIR_HEREDOC, "REDIR_HEREDOC"},
	{TOK_EOF, "EOF"},
	{TOK_NEWLINE, "NEWLINE"},
	{TOK_WORD, "WORD"},
	{TOK_DELIM, "DELIM"},
	{TOK_OPERATOR, "OPERATOR"},
	{TOK_HEREDOC_BODY, "HEREDOC_BODY"},
	{0, NULL}
	};

	return (lookup_name(names, type, "      UNKNOWN"));
}

char	*debug_get_tok_kind(t_tk_kind kind)
{
	static const t_debug_name	names[] = {
	{TKD_OPERAND, "OPERAND"},
	{TKD_OPERATOR, "OPERATOR"},
	{TKD_STRUCT, "STRUCT"},
	{0, NULL}
	};

	return (lookup_name(names, kind, "UNKNOWN"));
}

char	*get_tok_family(t_tk_family family)
{
	static const t_debug_name	names[] = {
	{TKF_WORD, "         WORD"},
	{TKF_OPERATOR, "     OPERATOR"},
	{TKF_REDIRECT, "     REDIRECT"},
	{TKF_HEREDOC_BODY, " HEREDOC_BODY"},
	{TKF_SUBSHELL, "     SUBSHELL"},
	{TKF_WORD_REDIR, "   WORD_REDIR"},
	{0, NULL}
	};

	return (lookup_name(names, family, "      UNKNOWN"));
}

char	*get_ast_typestr(t_ast_type type)
{
	static const t_debug_name	names[] = {
	{AST_CMD, "CMD"},
	{AST_PIPE, "PIPE"},
	{AST_AND, "AND"},
	{AST_OR, "OR"},
	{AST_HEREDOC, "HEREDOC"},
	{AST_SUBSHELL, "SUBSHELL"},
	{AST_SUBSH_MARK, "SUBSH_MARK"},
	{AST_SUBSH_END, "SUBSH_END"},
	{AST_SEQ, "SEQUENCE"},
	{0, NULL}
	};

	return (lookup_name(names, type, "UNKNOWN"));
}

char	*convert_dbg_lvl_name(int lvl)
{
	static const t_debug_name	names[] = {
	{LVL_LEXER, "🔮 LXR"},
	{LVL_HEREDOC, "📥 HDC"},
	{LVL_PARSER, "🧩 PRS"},
	{LVL_AST, "🌳 AST"},
	{LVL_EXPAND, "🔰 EXP"},
	{LVL_ARGV, "📋 ARG"},
	{LVL_EXEC, "⚙️ EXC"},
	{LVL_OK, "📨 MSG"},
	{LVL_FAIL, "❌ ERR"},
	{LVL_CHK, "🔎 CHK"},
	{LVL_INPUT, "💬 INP"},
	{LVL_MNS, "🐰 MNS"},
	{0, NULL}
	};

	return (lookup_name(names, lvl, "🔥 UNK"));
}
