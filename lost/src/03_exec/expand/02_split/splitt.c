/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 01:43:43 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 20:46:48 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_expand.h"
#include "module_token.h"
#include "ft_std.h"

int	is_unescaped_delim(char *str, int pos, char *delims)
{
	int	i;
	int	backslashes;

	i = 0;
	while (delims[i])
	{
		if (str[pos] == delims[i])
		{
			backslashes = 0;
			while (pos - backslashes - 1 >= 0
				&& str[pos - backslashes - 1] == '\\')
				backslashes++;
			if (backslashes % 2 == 0)
				return (1);
		}
		i++;
	}
	return (0);
}

int	get_nb_parts(char *str, char *delims)
{
	int	i;
	int	count;
	int	in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (str[i])
	{
		if (is_unescaped_delim(str, i, delims))
			in_word = 0;
		else if (!in_word)
		{
			count++;
			in_word = 1;
		}
		i++;
	}
	return (count);
}

char	**split_unescaped(char *str, char *delims, int *nb)
{
	char	**out;
	int		i;
	int		j;
	int		start;

	*nb = get_nb_parts(str, delims);
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
		{
			out[j++] = dup_word(str + start, i - start);
		}
	}
	out[j] = NULL;
	return (out);
}

void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}
