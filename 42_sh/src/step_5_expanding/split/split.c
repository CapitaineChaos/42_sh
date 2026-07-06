/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 01:43:43 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 21:13:14 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_assembly.h"
#include "module_debug.h"
#include "module_expand.h"
#include "module_token.h"

int	is_unescaped_delim(char *str, int pos, char *delims)
{
	int	backslashes;

	if (!strchr(delims, str[pos]))
		return (0);
	backslashes = 0;
	while (pos - backslashes - 1 >= 0 && str[pos - backslashes - 1] == '\\')
		backslashes++;
	return (backslashes % 2 == 0);
}

static bool	part_needs_split(t_tk_part *part)
{
	int	i;

	if (part->type != TOK_UQUOTE || !part->str)
		return (false);
	i = 0;
	while (part->str[i])
	{
		if (is_unescaped_delim(part->str, i, " \t\n"))
			return (true);
		i++;
	}
	return (false);
}

static char	**split_unescaped(char *str, char *delims, int *nb)
{
	char	**out;
	int		i;
	int		j;
	int		start;

	*nb = 0;
	i = -1;
	while (str[++i])
		if (!is_unescaped_delim(str, i, delims)
			&& (i == 0 || is_unescaped_delim(str, i - 1, delims)))
			(*nb)++;
	out = malloc((*nb + 1) * sizeof(char *));
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		while (str[i] && is_unescaped_delim(str, i, delims))
			i++;
		start = i;
		while (str[i] && !is_unescaped_delim(str, i, delims))
			i++;
		if (i > start)
			out[j++] = strndup(str + start, i - start);
	}
	out[j] = NULL;
	return (out);
}

static void	split_part(t_tokens *out, t_token *current, t_tk_part *part)
{
	int			n;
	int			i;
	char		**words;
	t_tk_part	*p;

	words = split_unescaped(part->str, " \t\n", &n);
	i = 0;
	while (i < n)
	{
		if (*words[i])
		{
			if (!current)
			{
				current = token_new(TOK_WORD);
				tk_list_append(out, current);
			}
			p = tk_part_new(TOK_UQUOTE, 0);
			p->str = strdup(words[i]);
			tk_append_part_tok(current, p);
			if (i + 1 < n)
				current = NULL;
		}
		i++;
	}
	free_char_array(words);
}

static void	do_not_split(t_tokens *out, t_token *current, t_tk_part *part)
{
	t_tk_part	*next;

	if (!current)
	{
		current = token_new(TOK_WORD);
		tk_list_append(out, current);
	}
	next = part->next;
	part->next = NULL;
	part->prev = NULL;
	tk_append_part_tok(current, part);
	if (next)
		part = next->prev;
	else
		part = NULL;
}

static t_tokens	split_parts(t_token *tok)
{
	t_tokens	out;
	t_token		*current;
	t_tk_part	*part;
	t_tk_part	*next;

	memset(&out, 0, sizeof(t_tokens));
	current = NULL;
	part = tok->head;
	while (part)
	{
		next = part->next;
		if (part->type == TOK_UQUOTE && part->str && part->str[0] == '\0')
			free_token_part(part);
		else if (part_needs_split(part))
			split_part(&out, current, part);
		else
			do_not_split(&out, current, part);
		part = next;
	}
	return (out);
}

static int	split_token(t_tokens *tokens, t_token *cur)
{
	t_tokens	sub;
	t_token		*prev;
	t_token		*next;

	sub = split_parts(cur);
	if (sub.count > 0)
		replace_in_list(tokens, cur, sub.head);
	else
	{
		prev = cur->prev;
		next = cur->next;
		if (prev)
			prev->next = next;
		else
			tokens->head = next;
		if (next)
			next->prev = prev;
		else
			tokens->tail = prev;
		tokens->count--;
		free(cur);
	}
	trace_info_nvnb(LVL_EXPAND, "Splitted token in parts", sub.count);
	return (sub.count);
}

static bool	need_split(t_tk_part *p)
{
	while (p)
	{
		if (part_needs_split(p))
			return (true);
		if (p->type == TOK_UQUOTE && p->str && p->str[0] == '\0')
			return (true);
		p = p->next;
	}
	return (false);
}

int	split_tokens(t_tokens *tokens, bool is_redir)
{
	t_token	*cur;
	t_token	*next;
	int		n;

	cur = tokens->head;
	while (cur)
	{
		next = cur->next;
		if (cur->kind == TKD_OPERAND && cur->family == TKF_WORD
			&& !cur->has_quoted_part && need_split(cur->head))
		{
			n = split_token(tokens, cur);
			if (is_redir && n != 1)
				return (n);
		}
		cur = next;
	}
	return (-1);
}
