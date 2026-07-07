/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_content.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 02:48:50 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 00:14:28 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_token.h"
#include "module_minishell.h"

char	*slice_dup(const char *src, size_t start, size_t end)
{
	char	*dup;
	size_t	len;

	if (src == NULL || end < start)
		return (NULL);
	len = end - start;
	dup = malloc(len + 1);
	if (!dup)
		free_and_exit_minishell(EXIT_FAILURE);
	memcpy(dup, src + start, len);
	dup[len] = '\0';
	return (dup);
}

static size_t	calc_total_lenline(t_tk_part *wp)
{
	size_t	len;

	len = 0;
	while (wp)
	{
		if (wp->end >= wp->start)
			len += wp->end - wp->start;
		wp = wp->next;
	}
	return (len);
}

char	*aggregate_wordparts_to_strline(t_tk_part *first, char *line)
{
	t_tk_part	*part;
	size_t		total_len;
	size_t		len;
	size_t		i;
	char		*s;

	total_len = calc_total_lenline(first);
	s = malloc(total_len + 1);
	if (!s)
		free_and_exit_minishell(EXIT_FAILURE);
	i = 0;
	part = first;
	while (part)
	{
		if (part->end >= part->start)
		{
			len = part->end - part->start;
			memcpy(s + i, line + part->start, len);
			i += len;
		}
		part = part->next;
	}
	s[i] = '\0';
	return (s);
}
