/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 20:36:37 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 21:26:02 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "module_env.h"
#include "module_minishell.h"
#include "ft_std.h"
#include "utils.h"
#include "module_debug.h"
#include "all_config.h"

void	hide_node_key(t_env_lst *lst, const char *key)
{
	t_env_node	*cur;

	cur = lst->head;
	while (cur)
	{
		if (strcmp(cur->key, key) == 0)
		{
			cur->hidden = true;
			return ;
		}
		cur = cur->next;
	}
}

char	*get_env_entry(t_env_node *cur, ssize_t len1, ssize_t len2)
{
	char	*entry;

	if (cur->value != NULL)
	{
		len1 = strlen(cur->key) + 1;
		len2 = strlen(cur->value);
		entry = malloc(len1 + len2 + 1);
		if (!entry)
			return (NULL);
		ft_strlcpy(entry, cur->key, len1);
		entry[len1 - 1] = '=';
		ft_strlcpy(entry + len1, cur->value, len2 + 1);	
	}
	else
	{
		len1 = strlen(cur->key) + 1;
		entry = malloc(len1 + 1);
		if (!entry)
			return (NULL);
		ft_strlcpy(entry, cur->key, len1);
		entry[len1] = '=';
	}
	entry[len1 + len2] = '\0';
	return (entry);
}

size_t	get_envp_final_size(t_env_lst *lst)
{
	t_env_node	*cur;
	size_t		size;

	size = 0;
	cur = lst->head;
	while (cur)
	{
		size++;
		cur = cur->next;
	}
	return (size);
}

char	**env_list_to_envp(t_env_lst *lst)
{
	char		**envp;
	t_env_node	*cur;
	size_t		i;
	size_t		len;

	len = get_envp_final_size(lst);
	envp = malloc((len + 1) * sizeof(*envp));
	if (!envp)
		return (NULL);
	i = 0;
	cur = lst->head;
	while (cur)
	{
		envp[i] = get_env_entry(cur, 0, 0);
		if (!envp[i])
		{
			free_char_array(envp);
			free_and_exit_minishell(EXIT_FAILURE);
		}
		i++;
		cur = cur->next;
	}
	trace_info_nvnb(LVL_EXEC, "Envp size", i);
	envp[len] = NULL;
	return (envp);
}

void	envp_to_env_list(t_env_lst *lst, char **envp)
{
	size_t		i;
	char		*entry;
	char		*eq;

	lst->head = NULL;
	lst->tail = NULL;
	lst->size = 0;
	i = 0;
	while (envp[i])
	{
		entry = strdup(envp[i]);
		eq = strchr(entry, '=');
		if (eq)
			env_list_set(lst, entry, eq + 1);
		else
			env_list_set(lst, entry, "");
		free(entry);
		i++;
	}
}
