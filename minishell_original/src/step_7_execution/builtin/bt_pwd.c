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
#include "ft_wput.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

int	check_pwd_args(t_mns *mns, int argc, char **argv, char **envp)
{
	t_logger	logger;

	(void)mns;
	(void)argc;
	(void)envp;
	if (argc < 2 || !ft_strcmp(argv[1], "-") || !ft_strcmp(argv[1], "--"))
		return (0);
	else if (!ft_strncmp(argv[1], "-", 1))
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
	if (err == ENOMEM)
		write(STDERR_FILENO, "pwd: cannot allocate memory\n", 28);
	else if (err == EACCES)
		write (STDERR_FILENO, "pwd: error retrieving current directory: "
			"getcwd: cannot access parent directories:"
			" Permission denied\n", 102);
	else
		write(STDERR_FILENO, "pwd: error retrieving current directory:"
			" getcwd: cannot access parent directories:"
			" No such file or directory\n", 109);
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

static int	check_cwd_env(t_mns *mns, char **phys, bool opt_P, char **pwd)
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
	*pwd = ft_getenv(&mns->env, "PWD");
	return (-1);
}

int	builtin_pwd(t_mns *mns, int argc, char **argv, char **envp)
{
	char	*phys;
	char	*pwd;
	bool	opt_p;
	bool	opt_l;
	int		ret;

	opt_p = false;
	opt_l = false;
	if (argc == 2 && !ft_strcmp(argv[1], "-P"))
		opt_p = true;
	else if (argc == 2 && !ft_strcmp(argv[1], "-L"))
		opt_l = true;
	else if (check_pwd_args(mns, argc, argv, envp) != 0)
		return (2);
	ret = check_cwd_env(mns, &phys, opt_p, &pwd);
	if (ret != -1)
		return (ret);
	if (opt_l && pwd && *pwd)
		return (pwd_print_phys(pwd), free(phys), 0);
	if (pwd && *pwd && (!ft_strcmp(pwd, phys) || (ft_strncmp(pwd, "//", 2) == 0
				&& phys[0] == '/' && phys[1] == '\0')))
		pwd_print_phys(pwd);
	else
		pwd_print_phys(phys);
	return (free(phys), 0);
}
