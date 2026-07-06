/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 19:10:21 by cmassol           #+#    #+#             */
/*   Updated: 2025/05/29 19:59:21 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_builtin.h"
#include "module_env.h"
#include "ft_wput.h"
#include "module_minishell.h"
#include <stdio.h>
#include <unistd.h>

static int	cd_update_pwd(t_mns *mns, char *oldpwd, const char *arg_raw)
{
	char	*phys;
	char	*newpwd;

	phys = getcwd(NULL, 0);
	if (!phys)
		phys = strdup("");
	if (arg_raw && arg_raw[0] == '/' && arg_raw[1] == '/' && arg_raw[2] != '/')
	{
		if (arg_raw[2] == '\0')
			newpwd = strdup("//");
		else
			newpwd = ft_strjoin("//", phys + 1);
		free(phys);
	}
	else
		newpwd = phys;
	env_list_set(&mns->env, "OLDPWD", oldpwd);
	env_list_set(&mns->env, "PWD", newpwd);
	free(oldpwd);
	free(newpwd);
	return (0);
}

static int	check_args(t_mns *mns, int argc, char **argv, char **envp)
{
	t_logger	logger;

	(void)mns;
	(void)envp;
	if (argc > 2)
	{
		log_init(&logger);
		log_puts(&logger, "🐰: cd: too many arguments\n");
		log_flush(STDERR_FILENO, &logger, false);
		return (1);
	}
	if (argc == 2 && argv[1][0] == '-' && argv[1][1] && strcmp(argv[1], "-")
		&& strcmp(argv[1], "--"))
	{
		log_init(&logger);
		log_puts(&logger, "🐰: cd: ");
		log_puts(&logger, argv[1]);
		log_puts(&logger, ": invalid option\n");
		log_puts(&logger, "cd: cd [-L|[-P [-e]] [-@]] [dir]\n");
		log_flush(STDERR_FILENO, &logger, false);
		return (2);
	}
	return (0);
}

const char	*get_target(t_mns *mns, int argc, char **argv)
{
	const char	*target;
	t_logger	logger;

	if (argc == 1 || (argc == 2 && !strcmp(argv[1], "--")))
		target = ft_getenv(&mns->env, "HOME");
	else if (argc == 2 && !strcmp(argv[1], "-"))
	{
		target = ft_getenv(&mns->env, "OLDPWD");
		if (target)
		{
			log_init(&logger);
			log_puts(&logger, target);
			log_puts(&logger, "\n");
			log_flush(STDOUT_FILENO, &logger, false);
		}
	}
	else
		target = argv[1];
	return (target);
}

static int	check_target(const char **target, int argc, char **argv, char *old)
{
	t_logger	logger;

	if (!*target)
	{
		log_init(&logger);
		if (argc == 1 || !strcmp(argv[1], "--"))
			log_puts(&logger, "🐰: cd: HOME not set\n");
		else
			log_puts(&logger, "🐰: cd: OLDPWD not set\n");
		log_flush(STDERR_FILENO, &logger, false);
		free(old);
		return (1);
	}
	if ((*target)[0] == '\0')
		*target = ".";
	if (chdir(*target) != 0)
	{
		perror("cd");
		free(old);
		return (1);
	}
	return (0);
}

int	builtin_cd(t_mns *mns, int argc, char **argv, char **envp)
{
	const char	*target;
	char		*save_oldpwd;
	char		*arg_raw;
	int			ret;

	ret = check_args(mns, argc, argv, envp);
	if (ret != 0)
		return (ret);
	save_oldpwd = getcwd(NULL, 0);
	if (!save_oldpwd)
		save_oldpwd = strdup("");
	target = get_target(mns, argc, argv);
	ret = check_target(&target, argc, argv, save_oldpwd);
	if (ret != 0)
		return (ret);
	if (argc >= 2)
		arg_raw = argv[1];
	else
		arg_raw = (char *)target;
	return (cd_update_pwd(mns, save_oldpwd, arg_raw));
}
