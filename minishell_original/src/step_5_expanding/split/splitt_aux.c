/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitt_aux.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 01:15:15 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 21:10:28 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_expand.h"
#include "module_token.h"
#include "ft_std.h"

/**
 * @brief Vérifie si la part a besoin d'être splittée.
 * Une part doit être splittée si elle est de type TOK_UNQUOTED
 * et contient un espace non échappé.
 */
bool	part_needs_split(t_tk_part *part)
{
	return (part->type == TOK_UQUOTE
		&& find_unescaped_char(part->str, ' ') != -1);
}

bool	is_splittable_part(t_tk_part *part)
{
	char	*str;
	int		i;

	if (part->type != TOK_UQUOTE)
		return (false);
	str = part->str;
	i = 0;
	while (str[i])
	{
		if (is_unescaped_delim(str, i, " \t\n"))
			return (true);
		i++;
	}
	return (false);
}

char	*dup_word(char *start, int len)
{
	char	*res;
	int		i;

	res = malloc(len + 1);
	if (!res)
		return (NULL);
	i = 0;
	while (i < len)
	{
		res[i] = start[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}
