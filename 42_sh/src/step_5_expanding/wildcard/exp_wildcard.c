/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_wildcard.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 23:46:37 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 20:12:50 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_token.h"
#include "module_expand.h"

/**
 * @brief Fonction de correspondance de motifs avec `*`.
 * @param pattern Le motif à vérifier.
 * @param str La chaîne à vérifier.
 * @return true Si la chaîne correspond au motif.
 * @return false Sinon.
 */
static bool	match_star(const char *pattern, const char *str)
{
	if (*pattern == '\0')
		return (*str == '\0');
	if (*pattern == '*')
	{
		return (match_star(pattern + 1, str)
			|| (*str && match_star(pattern, str + 1)));
	}
	if (*pattern == *str)
	{
		return (match_star(pattern + 1, str + 1));
	}
	return (false);
}

static char	*last_slash(const char *s)
{
	char	*last;

	last = NULL;
	while (*s)
	{
		if (*s == '/')
			last = (char *)s;
		s++;
	}
	return (last);
}

char	*get_dir_path(const char *pattern)
{
	char	*slash_pos;
	char	*dir_path;

	slash_pos = last_slash(pattern);
	if (slash_pos)
		dir_path = ft_substr(pattern, 0, slash_pos - pattern + 1);
	else
		dir_path = strdup("./");
	return (dir_path);
}

const char	*get_file_pattern(const char *pattern)
{
	char	*slash_pos;

	slash_pos = last_slash(pattern);
	if (slash_pos)
		return (slash_pos + 1);
	else
		return (pattern);
}

int	add_matches_from_dir(t_token **result, DIR *dir, char *dir_path,
								const char *file_pattern)
{
	struct dirent	*entry;
	char			*full_path;
	bool			show_prefix;
	int				i;

	i = 0;
	if (strcmp(dir_path, "./") == 0)
		show_prefix = false;
	else
		show_prefix = true;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && match_star(file_pattern, entry->d_name))
		{
			if (show_prefix)
				full_path = ft_strjoin(dir_path, entry->d_name);
			else
				full_path = strdup(entry->d_name);
			insert_sorted(result, tk_glob_emit(full_path));
			i++;
		}
		entry = readdir(dir);
	}
	return (i);
}
