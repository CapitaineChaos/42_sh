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
#include "helpers.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char	**environ;

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
 * @brief Marque une variable comme exportée (environ).
 * @note "KEY=VALUE" -> setenv ; "KEY" seul sans valeur ne peut pas être
 *       représenté dans environ, on ne fait que valider l'identifiant.
 * @note validation POSIX [A-Za-z_][A-Za-z0-9_]*
 */
static bool	try_set_env(char *arg)
{
	char	*eq;

	eq = strchr(arg, '=');
	if (eq)
	{
		*eq = '\0';
		if (!is_valid_id(arg))
		{
			*eq = '=';
			return (print_invalid_id(arg));
		}
		setenv(arg, eq + 1, 1);
		*eq = '=';
	}
	else if (!is_valid_id(arg))
		return (print_invalid_id(arg));
	return (true);
}

/* Valeur entre "..." avec échappement bash : \ " $ ` sont préfixés d'un \. */
static void	export_put_value(t_logger *logger, const char *val)
{
	log_puts(logger, "=\"");
	while (*val)
	{
		if (*val == '"' || *val == '\\' || *val == '$' || *val == '`')
			log_putch(logger, '\\');
		log_putch(logger, *val);
		val++;
	}
	log_puts(logger, "\"");
}

static void	export_print_one(const char *entry)
{
	t_logger	logger;
	char		*dup;
	char		*eq;

	dup = strdup(entry);
	if (!dup)
		return ;
	eq = strchr(dup, '=');
	log_init(&logger);
	log_puts(&logger, "export ");
	if (eq)
	{
		*eq = '\0';
		log_puts(&logger, dup);
		export_put_value(&logger, eq + 1);
	}
	else
		log_puts(&logger, dup);
	log_puts(&logger, "\n");
	log_flush(STDOUT_FILENO, &logger, false);
	trace_logger_flush(-1, &logger, true);
	free(dup);
}

static int	cmp_env(const void *a, const void *b)
{
	return (strcmp(*(char *const *)a, *(char *const *)b));
}

static void	export_list_print(void)
{
	char	**arr;
	int		n;
	int		i;

	n = 0;
	while (environ[n])
		n++;
	arr = malloc((size_t)(n + 1) * sizeof(*arr));
	if (!arr)
		return ;
	i = -1;
	while (++i < n)
		arr[i] = environ[i];
	qsort(arr, (size_t)n, sizeof(*arr), cmp_env);
	i = -1;
	while (++i < n)
		export_print_one(arr[i]);
	free(arr);
}

static void	print_invalid_export(char **envp)
{
	t_logger	logger;

	(void)envp;
	log_init(&logger);
	log_puts(&logger, "🐰: export: invalid option\n");
	log_puts(&logger,
		"export: usage: export [-fn] [name[=value] ...] or export -p [-f]\n");
	log_flush(STDERR_FILENO, &logger, false);
	trace_logger_flush(-1, &logger, true);
}

int	builtin_export(t_mns *mns, int argc, char **argv, char **envp)
{
	int	i;
	int	status;

	(void)mns;
	if (!argv[1] || (strcmp(argv[1], "--") == 0 && argc == 2))
		return (export_list_print(), 0);
	if ((strncmp(argv[1], "--", 2) == 0 && argv[1][2] != '\0')
		|| (argv[1][0] == '-' && argv[1][1] && argv[1][1] != '-'))
	{
		print_invalid_export(envp);
		return (2);
	}
	if ((strcmp(argv[1], "--") == 0))
		i = 2;
	else
		i = 1;
	status = 0;
	while (argv[i])
	{
		if (!try_set_env(argv[i]))
			status = 1;
		i++;
	}
	return (status);
}
