/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:42:09 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 20:37:07 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "texts.h"
#include "module_minishell.h"
#include "helpers.h"

char	*get_ps1(void)
{
	t_logger	lg;
	t_mns		*mns;
	int			shell_level;
	char		*ps1;

	mns = get_mns(NULL);
	if (mns->posix)
	{
		ps1 = getenv("PS1");
		return (strdup(ps1 ? ps1 : "$ "));
	}
	log_init(&lg);
	log_puts(&lg, PTREE YL LP RT);
	log_puts(&lg, " " LAPINOU);
	log_puts(&lg, WILDCAT " ");
	log_puts(&lg, MG WC RT PTREE);
	shell_level = get_shell_level(NULL) - 1;
	log_puts(&lg, PTREE GN MS);
	if (shell_level > 1)
	{
		log_puts(&lg, BGN "#");
		log_putlong(&lg, get_shell_level(NULL));
	}
	log_puts(&lg, RT PTREE);
	if (mns->last_exit_code != 0)
		log_puts(&lg, RD " $" RT " ");
	else
		log_puts(&lg, GN " $" RT " ");
	return (strdup(lg.buf));
}

char	*get_prompt(t_contexts *ctxs, int lv)
{
	char	*ps2;

	if (ctxs->count > 0 && lv > 1)
	{
		if (get_mns(NULL)->posix)
		{
			ps2 = getenv("PS2");
			return (strdup(ps2 ? ps2 : "> "));
		}
		return (convert_contexts_to_str_(ctxs));
	}
	return (get_ps1());
}
