/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_dot.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 02:41:37 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 21:07:52 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_builtin.h"
#include "module_exec.h"
#include "module_env.h"
#include "utils.h"
#include <stdio.h>

static int	disp_and_ret(char *msg, char *arg, int ret)
{
	t_logger	lg;

	log_init(&lg);
	log_puts(&lg, "🐰🐯 Minishell: .:");
	log_puts(&lg, arg);
	log_puts(&lg, ": ");
	log_puts(&lg, msg);
	log_puts(&lg, "\n");
	log_flush(STDERR_FILENO, &lg, false);
	#pragma region "DEBUG"
	log_flush(-1, &lg, true);
	#pragma endregion
	return (ret);
}

int	builtin_dot(t_mns *mns, int argc, char **argv, char **envp)
{
	FILE	*f;

	(void)mns;
	(void)envp;
	if (argc < 2)
		return (disp_msg_and_ret(".: filename argument required\n.: "
				"usage: . filename [arguments]\n", 2));
	if (access(argv[1], F_OK) != 0)
		return (disp_and_ret(argv[1], "No such file or directory\n", 1));
	if (access(argv[1], R_OK) != 0)
		return (disp_and_ret(argv[1], "Permission denied\n", 1));
	f = fopen(argv[1], "r");
	if (!f)
		return (disp_and_ret(argv[1], "Cannot open file\n", 1));
	fclose(f);
	return (disp_and_ret(argv[1], "Not implemented yet\n", 1));
}
