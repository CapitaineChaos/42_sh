/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_specs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/08 00:00:00 by codex            #+#    #+#             */
/*   Updated: 2026/07/08 00:00:00 by codex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_token.h"

const t_tk_spec	g_tok_specs[TOK_COUNT] = {
	[TOK_WORD]			= {"WORD",			NULL,	TF_WORD | TF_OPERAND, PREC_NONE},
	[TOK_PIPE]			= {"PIPE",			"|",	TF_CONTROL_OP | TF_PIPE_OP, PREC_PIPE},
	[TOK_AND_IF]		= {"AND_IF",		"&&",	TF_CONTROL_OP | TF_AND_OR_OP, PREC_AND_OR},
	[TOK_OR_IF]			= {"OR_IF",			"||",	TF_CONTROL_OP | TF_AND_OR_OP, PREC_AND_OR},
	[TOK_LPAREN]		= {"LPAREN",		"(",	TF_SUBSHELL, PREC_NONE},
	[TOK_RPAREN]		= {"RPAREN",		")",	TF_SUBSHELL, PREC_NONE},
	[TOK_SEMI]			= {"SEMI",			";",	TF_LIST_SEP | TF_TERMINATOR, PREC_LIST},
	[TOK_ESCAPE]		= {"ESCAPE",		"\\",	TF_SYNTHETIC, PREC_NONE},
	[TOK_REDIR_IN]		= {"REDIR_IN",		"<",	TF_REDIR | TF_OPERAND, PREC_NONE},
	[TOK_REDIR_OUT]		= {"REDIR_OUT",		">",	TF_REDIR | TF_OPERAND, PREC_NONE},
	[TOK_REDIR_APPEND]	= {"REDIR_APPEND",	">>",	TF_REDIR | TF_OPERAND, PREC_NONE},
	[TOK_REDIR_HEREDOC]	= {"REDIR_HEREDOC",	"<<",	TF_REDIR | TF_OPERAND, PREC_NONE},
	[TOK_HEREDOC_BODY]	= {"HEREDOC_BODY",	NULL,	TF_OPERAND | TF_SYNTHETIC, PREC_NONE},
	[TOK_EOF]			= {"EOF",			NULL,	TF_TERMINATOR | TF_SYNTHETIC, PREC_NONE},
	[TOK_NEWLINE]		= {"NEWLINE",		"\n",	TF_LIST_SEP | TF_TERMINATOR, PREC_LIST},
};

bool	tok_has(t_tk_type type, int flags)
{
	if (type < 0 || type >= TOK_COUNT)
		return (false);
	return ((g_tok_specs[type].flags & flags) == flags);
}

const char	*tok_name(t_tk_type type)
{
	if (type < 0 || type >= TOK_COUNT || g_tok_specs[type].name == NULL)
		return ("UNKNOWN");
	return (g_tok_specs[type].name);
}

const char	*part_name(t_tk_part_type type)
{
	static const char	*names[] = {
	[PART_UQUOTE] = "UQUOTE",
	[PART_DQUOTE] = "DQUOTE",
	[PART_SQUOTE] = "SQUOTE",
	[PART_GLOB] = "GLOB",
	};

	if (type < 0 || type > PART_GLOB || names[type] == NULL)
		return ("UNKNOWN");
	return (names[type]);
}
