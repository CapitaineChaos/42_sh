/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 02:37:45 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 21:06:49 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_builtin.h"
#include "ft_wput.h"
#include "utils.h"
#include <errno.h>

static bool	is_option_n(char *str)
{
	if (*str != '-')
		return (false);
	str++;
	if (*str != 'n')
		return (false);
	while (*str)
	{
		if (*str != 'n')
			return (false);
		str++;
	}
	return (true);
}

static bool	is_new_line(char **argv, int *i)
{
	bool	nl;

	nl = true;
	while (argv[*i] && is_option_n(argv[*i]))
	{
		(*i)++;
		nl = false;
	}
	return (nl);
}

int	test_err(int ret)
{
	int			err;
	t_logger	lg;

	if (ret > 0)
		return (0);
	err = -1 * ret;
	if (err == EPIPE)
	{
		log_init(&lg);
		log_puts(&lg, "🐰🐯 Minishell: ");
		log_puts(&lg, "Broken pipe\n");
		log_flush(STDERR_FILENO, &lg, false);
	}
	return (ret);
}

int	builtin_echo(t_mns *mns, int argc, char **argv, char **envp)
{
	int		i;
	bool	nl;

	(void)envp;
	(void)argc;
	(void)mns;
	i = 1;
	nl = is_new_line(argv, &i);
	while (argv[i] != NULL)
	{
		if (test_err(ft_wputstr_fd(argv[i], STDOUT_FILENO)) < 0)
			return (1);
		if (test_err(argv[i + 1] != NULL
				&& ft_wputstr_fd(" ", STDOUT_FILENO)) < 0)
			return (1);
		i++;
	}
	if (nl)
	{
		if (test_err(ft_wputstr_fd("\n", STDOUT_FILENO) < 0))
			return (1);
	}
	return (0);
}
