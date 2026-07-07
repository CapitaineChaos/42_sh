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
	return ((char *)tok_name(type));
}

char	*debug_get_part_type(t_tk_part_type type)
{
	return ((char *)part_name(type));
}

char	*debug_get_token_role(t_tk_role role)
{
	static const t_debug_name	names[] = {
	{TKR_NONE, "NONE"},
	{TKR_ARGUMENT, "ARGUMENT"},
	{TKR_REDIR_OP, "REDIR_OP"},
	{TKR_REDIR_TARGET, "REDIR_TARGET"},
	{TKR_HEREDOC_DELIM, "HEREDOC_DELIM"},
	{TKR_HEREDOC_BODY, "HEREDOC_BODY"},
	{0, NULL}
	};

	return (lookup_name(names, role, "UNKNOWN"));
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

/* Texte d'un token (parts concaténées, tronqué à 70 car). Panels debugview. */
void	get_token_content_stream(t_token *token, char *buf, size_t buf_size)
{
	t_tk_part	*part;
	size_t		i;
	size_t		j;
	char		*src;

	if (token == NULL || buf == NULL || buf_size < 5)
		return ;
	i = 0;
	part = token->head;
	while (part && i < buf_size - 1)
	{
		src = part->str;
		if (src == NULL && token->source != NULL)
			src = token->source + part->start;
		if (src)
		{
			j = 0;
			while ((part->str || part->start + j < part->end)
				&& src[j] && i < buf_size - 1 && i < 70)
				buf[i++] = src[j++];
			if (i == 70 && buf_size > 73)
			{
				buf[70] = '.';
				buf[71] = '.';
				buf[72] = '.';
				return ((void)(buf[73] = '\0'));
			}
		}
		part = part->next;
	}
	buf[i] = '\0';
}

/* Texte d'une part (tronqué à 70 car). Panels debugview. */
void	get_tkpart_content_stream(t_token *token, t_tk_part *part,
	char *buf, size_t buf_size)
{
	size_t	i;
	size_t	j;
	char	*src;

	if (part == NULL || buf == NULL || buf_size < 5)
		return ;
	i = 0;
	while (part && i < buf_size - 1)
	{
		src = part->str;
		if (src == NULL && token != NULL && token->source != NULL)
			src = token->source + part->start;
		if (src)
		{
			j = 0;
			while ((part->str || part->start + j < part->end)
				&& src[j] && i < buf_size - 1 && i < 70)
				buf[i++] = src[j++];
			if (i == 70 && buf_size > 73)
			{
				buf[70] = '.';
				buf[71] = '.';
				buf[72] = '.';
				return ((void)(buf[73] = '\0'));
			}
		}
		part = part->next;
	}
	buf[i] = '\0';
}
