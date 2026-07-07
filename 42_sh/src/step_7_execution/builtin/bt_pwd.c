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

static int	check_pwd_args(int argc, char **argv)
{
	t_logger	logger;

	if (argc < 2 || !strcmp(argv[1], "-") || !strcmp(argv[1], "--"))
		return (0);
	else if (!strncmp(argv[1], "-", 1))
	{
		log_init(&logger);
		log_puts(&logger, "🐰: pwd: invalid option\n");
		log_puts(&logger,
			"pwd: pwd [-LP]\n");
		log_flush(STDERR_FILENO, &logger, false);
		return (2);
	}
	return (0);
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
	char	*phys;
	char	*pwd;
	bool	opt_p;
	bool	opt_l;
	int		ret;

	(void)mns;
	(void)envp;
	opt_p = false;
	opt_l = false;
	if (argc == 2 && !strcmp(argv[1], "-P"))
		opt_p = true;
	else if (argc == 2 && !strcmp(argv[1], "-L"))
		opt_l = true;
	else if (check_pwd_args(argc, argv) != 0)
		return (2);
	ret = check_cwd_env(&phys, opt_p, &pwd);
	if (ret != -1)
		return (ret);
	if (opt_l && pwd && *pwd)
		return (pwd_print_phys(pwd), free(phys), 0);
	if (pwd && *pwd && (!strcmp(pwd, phys) || (strncmp(pwd, "//", 2) == 0
				&& phys[0] == '/' && phys[1] == '\0')))
		pwd_print_phys(pwd);
	else
		pwd_print_phys(phys);
	return (free(phys), 0);
}
