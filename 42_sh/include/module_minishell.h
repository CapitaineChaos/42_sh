/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_minishell.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 22:36:55 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 02:19:13 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODULE_MINISHELL_H
# define MODULE_MINISHELL_H

# include "module_minishell.h"
# include "module_lexer.h" 
# include "module_env.h"
# include "module_checker.h"
# include "module_parser.h"
# include "module_read.h"

# ifndef SIGNAL_HANDLER_H
#  define SIGNAL_HANDLER_H
#  include <signal.h>
# endif

typedef struct s_mns		t_mns;
typedef struct s_lexer		t_lexer;
typedef struct s_parser		t_parser;
typedef struct s_strapi		t_strapi;

typedef struct s_mns
{
	int						next_job_id;
	char					**argv;
	t_checker				checker;
	t_lexer					lexer;
	t_parser				parser;
	t_main_data				data;
	bool					read_exit;
	bool					posix;
	int						nb_lines;
	int						last_exit_code;
	int						argc;
	int						level;
}	t_mns;

/**
 * MINISHELL
 */

void	free_mns(t_mns *mns);
t_mns	*get_mns(t_mns *mns);
void	free_and_exit_minishell(intmax_t exit_code);
void	minishell(t_mns *mns);
void	init_mns(t_mns *mns, char *ifs, char **envp);

#endif
