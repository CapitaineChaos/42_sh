/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 14:36:06 by cmassol           #+#    #+#             */
/*   Updated: 2025/05/29 19:54:10 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_builtin.h"
#include "module_minishell.h"
#include "utils.h"
#include "module_env.h"
#include "helpers.h"

int	print_invalid_id(char *arg)
{
	t_logger	logger;

	log_init(&logger);
	log_puts(&logger, "export: `");
	log_puts(&logger, arg);
	log_puts(&logger, "': not a valid identifier\n");
	log_flush(STDERR_FILENO, &logger, false);
	trace_logger_flush(-1, &logger, true);
	return (0);
}

/**
 * @brief Set an environment variable in the environment list.
 * @param mns The minishell structure.
 * @param arg The argument string containing the variable name and value.
 * @return true if the variable was set successfully, false otherwise.
 * @note The argument string should be in the format "KEY=VALUE".
 * @note validation POSIX [A-Za-z_][A-Za-z0-9_]* 
 */
static bool	try_set_env(t_mns *mns, char *arg)
{
	char		*eq;
	char		*val;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		*eq = '\0';
		val = eq + 1;
		if (!is_valid_id(arg))
		{
			*eq = '=';
			return (print_invalid_id(arg));
		}
		env_list_set(&mns->env, arg, val);
		*eq = '=';
	}
	else
	{
		if (!is_valid_id(arg))
			return (print_invalid_id(arg));
		env_list_set(&mns->env, arg, NULL);
	}
	return (true);
}

static void	export_list_print(t_env_lst *lst)
{
	t_env_node	*cur;
	t_logger	logger;
	char		*value;

	cur = lst->head;
	while (cur)
	{
		if (!cur->hidden && ft_strcmp(cur->key, "_") && cur->value)
		{
			log_init(&logger);
			log_puts(&logger, "export ");
			log_puts(&logger, cur->key);
			value = cur->value;
			if (value)
			{
				log_puts(&logger, "=");
				log_puts(&logger, "\"");
				log_puts(&logger, value);
				log_puts(&logger, "\"");
			}
			log_puts(&logger, "\n");
			log_flush(STDOUT_FILENO, &logger, false);
			trace_logger_flush(-1, &logger, true);
		}
		cur = cur->next;
	}
}

static void	print_invalid_export(char **envp)
{
	t_logger	logger;

	(void)envp;
	log_init(&logger);
	log_puts(&logger, "🐰: export: invalid option\n");
	log_puts(&logger, "export: export [-fn] [name[=value] ...] or export -p\n");
	log_flush(STDERR_FILENO, &logger, false);
	trace_logger_flush(-1, &logger, true);
}

int	builtin_export(t_mns *mns, int argc, char **argv, char **envp)
{
	int			i;
	int			status;

	if (!argv[1] || (ft_strcmp(argv[1], "--") == 0 && argc == 2))
		return (export_list_print(&mns->env), 0);
	if ((ft_strncmp(argv[1], "--", 2) == 0 && argv[1][2] != '\0') ||
		(argv[1][0] == '-' && argv[1][1] && argv[1][1] != '-'))
	{
		print_invalid_export(envp);
		return (2);
	}
	if ((ft_strcmp(argv[1], "--") == 0))
		i = 2;
	else
		i = 1;
	status = 0;
	while (argv[i])
	{
		if (!try_set_env(mns, argv[i]))
			status = 1;
		i++;
	}
	return (status);
}
