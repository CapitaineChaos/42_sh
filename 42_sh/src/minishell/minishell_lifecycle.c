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
	parser_free(&mns->parser);
	lexer_free(&mns->lexer);
	sstr_free(&mns->data.inputs);
	free(mns->data.stream);
	free(mns->data.ifs);
	rl_clear_history();
}

void	init_modules(t_mns *mns, char *ifs, char **envp)
{
	mns->checker = (t_checker){0};
	mns->lexer = (t_lexer){0};
	mns->parser = (t_parser){0};
	mns->data = (t_main_data){0};
	(void)envp;
	create_env(mns);
	checker_init(&mns->checker);
	lexer_init(&mns->lexer, ifs);
	parser_init(&mns->parser);
}

void	init_mns(t_mns *mns, char *ifs, char **envp)
{
	init_modules(mns, ifs, envp);
	mns->argv = NULL;
	mns->argc = 0;
	mns->read_exit = false;
	mns->nb_lines = 0;
	mns->last_exit_code = 0;
	mns->level = 0;
	mns->data.chk = &mns->checker;
	mns->data.lx = &mns->lexer;
	mns->data.lc = &mns->nb_lines;
	mns->data.stream = NULL;
	mns->data.inputs = NULL;
	mns->data.partial_input = NULL;
	mns->data.ifs = ifs;
	mns->checker.ifs = mns->data.ifs;
	mns->lexer.ifs = mns->data.ifs;
	mns->next_job_id = 1;
	get_mns(mns);
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
