/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_id.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 00:14:39 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/09 22:00:38 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "ft_std.h"

bool	is_valid_id(const char *s)
{
	size_t	i;

	if (!s || !(isalpha((unsigned char)s[0]) || s[0] == '_'))
		return (false);
	i = 1;
	while (s[i])
	{
		if (!(isalnum((unsigned char)s[i]) || s[i] == '_'))
			return (false);
		i++;
	}
	return (true);
}

char	*get_identifier(const char *str, size_t *idx)
{
	const char	*start;
	size_t		len;
	char		*id;

	if (!str || !idx)
		return (NULL);
	len = 0;
	if (!(isalpha((unsigned char)str[*idx]) || str[*idx] == '_'))
		return (NULL);
	start = str + *idx;
	while (str[*idx]
		&& (isalnum((unsigned char)str[*idx]) || str[*idx] == '_'))
	{
		(*idx)++;
		len++;
	}
	id = strndup(start, len);
	return (id);
}
