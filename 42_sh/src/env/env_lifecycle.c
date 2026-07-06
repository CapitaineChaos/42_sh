/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_lifecycle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:34:00 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/23 03:15:43 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_env.h"
#include "ft_std.h" 
#include <stdlib.h>
#include <stdbool.h>

void	env_list_unset(t_env_lst *lst, const char *key)
{
	t_env_node	*cur;

	cur = lst->head;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			if (cur->hidden == false)
				env_list_rem_node(lst, cur);
			return ;
		}
		cur = cur->next;
	}
}

void	env_list_free(t_env_lst *lst)
{
	t_env_node	*cur;
	t_env_node	*next;

	cur = lst->head;
	while (cur)
	{
		next = cur->next;
		free(cur->key);
		free(cur->value);
		free(cur);
		cur = next;
	}
	lst->head = NULL;
	lst->tail = NULL;
	lst->size = 0;
}

void	env_list_init(t_env_lst *lst, char **envp)
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
		entry = ft_strdup(envp[i]);
		eq = ft_strchr(entry, '=');
		*eq = '\0';
		env_list_set(lst, entry, eq + 1);
		free(entry);
		i++;
	}
}
