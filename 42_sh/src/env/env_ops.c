/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_ops.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 23:40:45 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/23 02:44:17 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_env.h"
#include "ft_std.h"
#include "module_minishell.h"

void	env_list_rem_node(t_env_lst *lst, t_env_node *node)
{
	if (node->prev)
		node->prev->next = node->next;
	else
		lst->head = node->next;
	if (node->next)
		node->next->prev = node->prev;
	else
		lst->tail = node->prev;
	free(node->key);
	free(node->value);
	free(node);
	lst->size--;
}

t_env_node	*env_list_create_node(const char *k, const char *value)
{
	t_env_node	*node;

	node = ft_calloc(1, sizeof(*node));
	if (!node)
		free_and_exit_minishell(EXIT_FAILURE);
	node->key = ft_strdup(k);
	if (value != NULL)
		node->value = ft_strdup(value);
	node->hidden = false;
	node->prev = NULL;
	node->next = NULL;
	return (node);
}

void	env_lst_add_entry(t_env_lst *lst, const char *k, const char *v)
{
	t_env_node	*node;

	node = env_list_create_node(k, v);
	node->hidden = false;
	if (lst->head == NULL)
	{
		lst->head = node;
		lst->tail = node;
	}
	else
	{
		lst->tail->next = node;
		node->prev = lst->tail;
		lst->tail = node;
	}
	lst->size++;
}

void	env_lst_add_entry_hid(t_env_lst *lst, const char *k, const char *v)
{
	t_env_node	*node;

	node = env_list_create_node(k, v);
	node->hidden = true;
	if (lst->head == NULL)
	{
		lst->head = node;
		lst->tail = node;
	}
	else
	{
		lst->tail->next = node;
		node->prev = lst->tail;
		lst->tail = node;
	}
	lst->size++;
}
