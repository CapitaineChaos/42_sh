/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_lifecycle.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 00:45:07 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 02:14:34 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "helpers.h"
#include "module_env.h"
#include "module_checker.h"
#include "module_lexer.h"
#include "module_parser.h"
#include "module_minishell.h"
#include "ft_strapi.h"
#include "utils.h"
#include "all_config.h"
#include <readline/history.h>
#include <readline/readline.h>

/**
 * @brief Libère les ressources allouées pour le minishell et quitte le
 * programme.
 * @see module_minishell.h
 * @param exit_code Le code de sortie à retourner au shell parent.
 */
void	free_and_exit_minishell(intmax_t exit_code)
{
	t_mns	*mns;

	trace_info(LVL_FAIL, "... Free and exit ...");
	mns = get_mns(NULL);
	if (mns)
	{
		free_mns(mns);
	}
	if (exit_code < 0)
		exit_code = 256 + exit_code;
	else if (exit_code > 255)
		exit_code = exit_code % 256;
	exit(exit_code);
}

void	free_mns(t_mns *mns)
{
	env_list_free(&mns->env);
	parser_free(&mns->parser, 255);
	lexer_free(&mns->lexer, 255);
	sstr_free(&mns->data.inputs);
	free(mns->data.stream);
	free(mns->data.ifs);
	rl_clear_history();
	trace_info(LVL_MNS, "Free mns");
}

void	init_modules(t_mns *mns, char *ifs, char **envp, int lv)
{
	mns->env = (t_env_lst){0};
	mns->checker = (t_checker){0};
	mns->lexer = (t_lexer){0};
	mns->parser = (t_parser){0};
	mns->data = (t_main_data){0};
	env_list_init(&mns->env, envp);
	create_env(mns);
	checker_init(&mns->checker, lv);
	lexer_init(&mns->lexer, ifs, lv);
	parser_init(&mns->parser, lv);
}

void	init_mns(t_mns *mns, char *ifs, char **envp, int lv)
{
	init_modules(mns, ifs, envp, lv);
	trace_start(LVL_MNS, "Init mns");
	mns->argv = NULL;
	mns->argc = 0;
	mns->read_exit = false;
	mns->nb_lines = 0;
	mns->last_exit_code = 0;
	mns->level = 0;
	mns->data.chk = &mns->checker;
	mns->data.lx = &mns->lexer;
	mns->data.lc = &mns->nb_lines;
	mns->data.lv = lv;
	mns->data.stream = NULL;
	mns->data.inputs = NULL;
	mns->data.partial_input = NULL;
	mns->data.ifs = ifs;
	mns->checker.ifs = mns->data.ifs;
	mns->lexer.ifs = mns->data.ifs;
	mns->next_job_id = 1;
	get_mns(mns);
	trace_ok(LVL_MNS, "Init mns done");
}

t_mns	*get_mns(t_mns *ptr)
{
	static t_mns	*stored;

	if (ptr != NULL && stored == NULL)
	{
		stored = ptr;
	}
	return (stored);
}
