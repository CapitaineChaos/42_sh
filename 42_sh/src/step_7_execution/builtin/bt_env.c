/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 18:35:15 by cmassol           #+#    #+#             */
/*   Updated: 2025/05/29 19:54:20 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_minishell.h"
#include "module_env.h"
#include "module_exec.h"
#include <stdio.h>

int	env_list_print(t_env_lst *lst)
{
	t_env_node	*cur;
	t_logger	lg;
	char		*value;

	cur = lst->head;
	while (cur)
	{
		if (!cur->hidden)
		{
			value = cur->value;
			if (value != NULL)
			{
				log_init(&lg);
				log_puts(&lg, cur->key);
				log_puts(&lg, "=");
				log_puts(&lg, value);
				log_puts(&lg, "\n");
				if (write_buffer(STDOUT_FILENO, &lg) != 0)
					return (1);
			}
		}
		cur = cur->next;
	}
	return (0);
}

int	builtin_env(t_mns *mns, int argc, char **argv, char **envp)
{
	(void)argv;
	(void)envp;
	(void)argc;
	return (env_list_print(&mns->env));
}
