/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 04:55:40 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/27 23:58:35 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "module_env.h"
#include "module_minishell.h"

static void	chk_env(const char *key, const char *content)
{
	if (!getenv(key))
		setenv(key, content, 1);
}

static long	next_shlvl(void)
{
	char	*cur;
	char	*end;
	long	n;

	cur = getenv("SHLVL");
	if (!cur)
		return (1);
	n = strtol(cur, &end, 10);
	while (*end == ' ' || *end == '\t')
		end++;
	if (*end != '\0' || n >= 1000)
		return (1);
	if (n < 0)
		return (0);
	return (n + 1);
}

static void	update_shell_level(t_mns *mns)
{
	char	*tmp;
	long	n;

	n = next_shlvl();
	mns->level = (int)n;
	if (asprintf(&tmp, "%ld", n) < 0)
		return ;
	setenv("SHLVL", tmp, 1);
	free(tmp);
}

void	create_env(t_mns *mns)
{
	char	*cwd;

	if (isatty(STDIN_FILENO))
		unsetenv("_");
	chk_env("PATH", "/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin");
	cwd = getcwd(NULL, 0);
	if (cwd)
		chk_env("PWD", cwd);
	free(cwd);
	update_shell_level(mns);
}
