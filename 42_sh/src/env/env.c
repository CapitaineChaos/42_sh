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

/* Niveau SHLVL suivant, robuste face à une valeur héritée pourrie. long +
 * strtol pour ne jamais déborder (un int déborderait sur SHLVL=2147483647).
 * Calé sur bash : non numérique / poubelle en fin / >= 1000 -> reset (1) ;
 * négatif -> 0 ; sinon n + 1. */
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

	chk_env("PATH", "/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin");
	chk_env("TERM", "c");
	chk_env("LANG", "C.UTF-8");
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		chk_env("PWD", cwd);
		chk_env("OLDPWD", cwd);
		chk_env("HOME", cwd);
	}
	chk_env("USER", "ancientacodearchitects");
	free(cwd);
	update_shell_level(mns);
}
