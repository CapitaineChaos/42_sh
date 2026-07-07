/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 04:06:41 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/23 03:08:24 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_builtin.h"
#include "module_minishell.h"
#include "texts.h"
#include "ft_std.h"
#include <stdio.h>

static t_builtin_entry	*get_builtin_functions(void)
{
	static t_builtin_entry	builtins[] = {
	{".", builtin_dot},		
	{"cd", builtin_cd},
	{"pwd", builtin_pwd},
	{"echo", builtin_echo},
	{"exit", builtin_exit},
	{"export", builtin_export},
	{"unset", builtin_unset},
	{NULL, NULL}
	};

	return (builtins);
}

static t_builtin_entry	*find_builtin(char *cmd)
{
	t_builtin_entry	*builtins;
	int				i;

	builtins = get_builtin_functions();
	i = 0;
	while (builtins[i].fn != NULL)
	{
		if (strcmp(cmd, builtins[i].name) == 0)
			return (&builtins[i]);
		i++;
	}
	return (NULL);
}

bool	is_builtin(char *cmd)
{
	t_builtin_entry	*found;

	found = find_builtin(cmd);
	if (found)
	{
		trace_info_nvstr(LVL_EXEC, "    >  Found Built-in name",
			(char *)found->name);
		return (true);
	}
	trace_info_nvstr(LVL_EXEC, "    > No Built-in found", cmd);
	return (false);
}

/**
 * @brief Execute a built-in command.
 * 
 * @param argv The arguments for the built-in command.
 * @return int 0 if the built-in was executed successfully, 1 otherwise.
 */
int	execute_builtin(int argc, char **argv)
{
	t_builtin_entry	*found;

	found = find_builtin(argv[0]);
	if (found)
		return (found->fn(get_mns(NULL), argc, argv, NULL));
	return (1);
}
