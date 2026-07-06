/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:45:19 by smaitre           #+#    #+#             */
/*   Updated: 2025/04/29 21:23:18 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_std.h"
#include <stdlib.h>

static const char	*get_start(const char *s, char c)
{
	while (*s && *s == c)
		s++;
	return (s);
}

static size_t	get_nb(const char *s, char c)
{
	int	nb;

	nb = 0;
	while (*s)
	{
		s = get_start(s, c);
		if (*s)
			nb++;
		s = strchr(s, c);
		if (!s)
			return (nb);
	}
	return (nb);
}

static void	*free_tab(char **tab, size_t pos)
{
	size_t	i;

	i = 0;
	while (i < pos)
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

static size_t	get_wdlen(const char *s, char c)
{
	size_t		len;
	const char	*end;

	end = strchr(s, c);
	if (!end)
		end = strchr(s, '\0');
	len = (size_t)(end - s);
	return (len);
}

char	**ft_split(char const *s, char c)
{
	size_t	nbstr;
	size_t	wdlen;
	size_t	i;
	char	**tab;

	nbstr = get_nb(s, c);
	tab = (char **)malloc((nbstr + 1) * sizeof (char *));
	if (!tab)
		return (NULL);
	tab[nbstr] = NULL;
	i = 0;
	while (i < nbstr)
	{
		s = get_start(s, c);
		wdlen = get_wdlen(s, c);
		tab[i] = strndup(s, wdlen);
		s = s + wdlen;
		if (!tab[i])
			return (free_tab(tab, i));
		i++;
	}
	tab[i] = NULL;
	return (tab);
}
