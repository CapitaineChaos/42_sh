/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 04:55:40 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/27 23:58:35 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "module_debug.h"
#include "module_env.h"
#include "module_minishell.h"
#include "helpers.h"
#include "ft_std.h"
#include <stdlib.h>
#include <unistd.h>

void	env_list_set(t_env_lst *lst, const char *key, const char *value)
{
	t_env_node	*cur;

	cur = lst->head;
	while (cur)
	{
		if (strcmp(cur->key, key) == 0)
		{
			free(cur->value);
			if (value == NULL)
				cur->value = NULL;
			else
				cur->value = strdup(value);
			return ;
		}
		cur = cur->next;
	}
	env_lst_add_entry(lst, key, value);
}

/**
 * @brief Vérifie si une variable d'environnement existe déjà dans la liste.
 * Si elle n'existe pas, elle est ajoutée.
 */
static void	chk_env(t_env_lst *lst, char *key, char *content, bool hidden)
{
	char	*tmp;

	tmp = ft_getenv(lst, key);
	if (!tmp)
	{
		if (hidden)
		{
			env_lst_add_entry_hid(lst, key, content);
		}
		else
		{
			env_lst_add_entry(lst, key, content);
		}
	}
}

void	update_shell_level(t_mns *mns, t_env_lst *lst)
{
	char	*tmp;
	int		n;

	tmp = ft_getenv(lst, "SHLVL");
	n = 0;
	if (tmp != NULL)
		n = atoi(tmp);
	if (n < 0)
		n = 0;
	n++;
	if (asprintf(&tmp, "%d", n) < 0)
		tmp = NULL;
	env_list_set(lst, "SHLVL", tmp);
	free(tmp);
	n = get_shell_level(mns) + 1;
	mns->level = n;
	if (asprintf(&tmp, "%d", n) < 0)
		tmp = NULL;
	env_list_set(lst, "LW_SHLVL", tmp);
	hide_node_key(lst, "LW_SHLVL");
	free(tmp);
}

void	create_env(t_mns *mns)
{
	char		*cwd;
	t_env_lst	*lst;

	lst = &mns->env;
	chk_env(lst, "PATH", "/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin", false);
	chk_env(lst, "TERM", "c", false);
	chk_env(lst, "LANG", "C.UTF-8", false);
	cwd = getcwd(NULL, 0);
	chk_env(lst, "PWD", cwd, false);
	chk_env(lst, "OLDPWD", cwd, false);
	chk_env(lst, "HOME", cwd, false);
	chk_env(lst, "USER", "ancientacodearchitects", false);
	env_list_set(lst, "LANGUAGE", "en");
	env_list_set(lst, "LC_ALL", "en_US.UTF-8");
	env_list_set(lst, "LANG", "en_US.UTF-8");
	update_shell_level(mns, lst);
	env_lst_reorder(lst);
	free(cwd);
}
