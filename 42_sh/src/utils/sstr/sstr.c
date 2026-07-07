/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sstr.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 03:03:56 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/23 02:26:22 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_checker.h"
#include "ft_std.h"
#include <stdlib.h>

size_t	sstrs_total_len(t_sstr *first_str)
{
	size_t	len;

	len = 0;
	while (first_str)
	{
		len += strlen(first_str->line);
		first_str = first_str->next;
	}
	return (len);
}

size_t	sstrs_count(t_sstr *first_str)
{
	size_t	count;

	count = 0;
	while (first_str)
	{
		count++;
		first_str = first_str->next;
	}
	return (count);
}

char	*sstrs_flatten_sep(t_sstr *first_str, char sep)
{
	t_sstr	*tmp;
	char	*str;
	char	*p;
	size_t	len;
	size_t	part_len;

	if (!first_str)
		return (NULL);
	len = sstrs_total_len(first_str);
	str = malloc(len + sstrs_count(first_str));
	if (!str)
		return (NULL);
	p = str;
	tmp = first_str;
	while (tmp)
	{
		part_len = strlen(tmp->line);
		memcpy(p, tmp->line, part_len);
		p += part_len;
		tmp = tmp->next;
		if (tmp)
			*p++ = sep;
	}
	*p = '\0';
	return (str);
}

char	*sstrs_flatten(t_sstr *first_str)
{
	t_sstr	*tmp;
	char	*str;
	char	*p;
	size_t	len;

	if (!first_str)
		return (NULL);
	len = sstrs_total_len(first_str);
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	p = str;
	tmp = first_str;
	while (tmp)
	{
		len = strlen(tmp->line);
		memcpy(p, tmp->line, len);
		p += len;
		tmp = tmp->next;
	}
	*p = '\0';
	return (str);
}

char	*sstrs_get_last(t_sstr *first_str)
{
	if (!first_str)
		return (NULL);
	while (first_str->next)
		first_str = first_str->next;
	return (first_str->line);
}

void	sstrs_append(t_sstr **strs, char *str)
{
	t_sstr	*new_line;
	t_sstr	*tmp;

	if (!strs || !str)
		return ;
	new_line = (t_sstr *)malloc(sizeof(t_sstr));
	if (!new_line)
		return ;
	new_line->line = str;
	new_line->next = NULL;
	if (*strs == NULL)
	{
		*strs = new_line;
		return ;
	}
	tmp = *strs;
	while (tmp->next)
	{
		tmp = tmp->next;
	}
	tmp->next = new_line;
}

void	sstr_free(t_sstr **strs)
{
	t_sstr	*tmp;
	t_sstr	*cur;

	if (!strs || !*strs)
		return ;
	cur = *strs;
	while (cur)
	{
		tmp = cur;
		cur = cur->next;
		if (tmp->line)
		{
			free(tmp->line);
			tmp->line = NULL;
		}
		free(tmp);
		tmp = NULL;
	}
	*strs = NULL;
}
