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
	[TOK_WORD]			= {"WORD",			NULL,	LCL_WORD,		TA_CMD_PART},
	[TOK_PIPE]			= {"PIPE",			"|",	LCL_CONTROL,	TA_BINARY},
	[TOK_AND_IF]		= {"AND_IF",		"&&",	LCL_CONTROL,	TA_BINARY | TA_ANDOR},
	[TOK_OR_IF]			= {"OR_IF",			"||",	LCL_CONTROL,	TA_BINARY | TA_ANDOR},
	[TOK_LPAREN]		= {"LPAREN",		"(",	LCL_CONTROL,	TA_NONE},
	[TOK_RPAREN]		= {"RPAREN",		")",	LCL_CONTROL,	TA_NONE},
	[TOK_SEMI]			= {"SEMI",			";",	LCL_CONTROL,	TA_SEP},
	[TOK_ESCAPE]		= {"ESCAPE",		"\\",	LCL_INTERNAL,	TA_NONE},
	[TOK_REDIR_IN]		= {"REDIR_IN",		"<",	LCL_REDIR,		TA_CMD_PART | TA_REDIR_IN},
	[TOK_REDIR_OUT]		= {"REDIR_OUT",		">",	LCL_REDIR,		TA_CMD_PART | TA_REDIR_OUT},
	[TOK_REDIR_APPEND]	= {"REDIR_APPEND",	">>",	LCL_REDIR,		TA_CMD_PART | TA_REDIR_OUT},
	[TOK_REDIR_HEREDOC]	= {"REDIR_HEREDOC",	"<<",	LCL_REDIR,		TA_CMD_PART | TA_REDIR_IN | TA_REDIR_HERE},
	[TOK_HEREDOC_BODY]	= {"HEREDOC_BODY",	NULL,	LCL_INTERNAL,	TA_CMD_PART},
	[TOK_EOF]			= {"EOF",			NULL,	LCL_INTERNAL,	TA_NONE},
	[TOK_NEWLINE]		= {"NEWLINE",		"\n",	LCL_CONTROL,	TA_SEP},
};

bool	tok_has(t_tk_type type, int attrs)
{
	if (type < 0 || type >= TOK_COUNT)
		return (false);
	return ((g_tok_specs[type].attrs & attrs) == attrs);
}

t_tk_class	tok_class(t_tk_type type)
{
	if (type < 0 || type >= TOK_COUNT)
		return (LCL_INTERNAL);
	return (g_tok_specs[type].cls);
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
