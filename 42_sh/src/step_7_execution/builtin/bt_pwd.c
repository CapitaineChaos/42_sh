/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 04:26:26 by cmassol           #+#    #+#             */
/*   Updated: 2025/05/29 21:06:22 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_builtin.h"
#include "module_minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

static int	pwd_bad_opt(char *opt)
{
	t_logger	logger;

	log_init(&logger);
	log_puts(&logger, "🐰: pwd: ");
	log_puts(&logger, opt);
	log_puts(&logger, ": invalid option\npwd: pwd [-LP]\n");
	log_flush(STDERR_FILENO, &logger, false);
	return (2);
}


static void	print_builtin_pwd_err(int err)
{
	const char	*msg;

	if (err == ENOMEM)
		msg = "pwd: cannot allocate memory\n";
	else if (err == EACCES)
		msg = "pwd: error retrieving current directory: "
			"getcwd: cannot access parent directories:"
			" Permission denied\n";
	else
		msg = "pwd: error retrieving current directory:"
			" getcwd: cannot access parent directories:"
			" No such file or directory\n";
	write(STDERR_FILENO, msg, strlen(msg));
}

static int	pwd_print_phys(char *my_path)
{
	char		*path;
	t_logger	logger;

	log_init(&logger);
	if (!my_path)
	{
		path = getcwd(NULL, 0);
		if (!path)
		{
			perror("pwd");
			print_builtin_pwd_err(errno);
			return (1);
		}
		log_puts(&logger, path);
	}
	else
		log_puts(&logger, my_path);
	log_puts(&logger, "\n");
	log_flush(STDOUT_FILENO, &logger, false);
	if (!my_path)
		free(path);
	return (0);
}

static int	check_cwd_env(char **phys, bool opt_P, char **pwd)
{
	*phys = getcwd(NULL, 0);
	if (!*phys)
	{
		print_builtin_pwd_err(errno);
		return (1);
	}
	if (opt_P)
	{
		pwd_print_phys(*phys);
		free(*phys);
		return (0);
	}
	*pwd = getenv("PWD");
	return (-1);
}

int	builtin_pwd(t_mns *mns, int argc, char **argv, char **envp)
{
	char		*phys;
	char		*pwd;
	int			ret;
	t_optres	opt;

	(void)mns;
	(void)envp;
	(void)argc;
	if (bt_getopt(argv, "LP", NULL, &opt) < 0)
		return (pwd_bad_opt(opt.badarg));
	ret = check_cwd_env(&phys, opt.last == 'P', &pwd);
	if (ret != -1)
		return (ret);
	if (pwd && *pwd && (!strcmp(pwd, phys) || (strncmp(pwd, "//", 2) == 0
				&& phys[0] == '/' && phys[1] == '\0')))
		pwd_print_phys(pwd);
	else
		pwd_print_phys(phys);
	return (free(phys), 0);
}
