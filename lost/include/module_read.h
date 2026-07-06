/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_read.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 18:43:56 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 04:55:19 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODULE_READ_H
# define MODULE_READ_H

# include "module_lexer.h"
# include "module_checker.h"
# include "ft_strapi.h"
# include "utils.h"
# include <signal.h>

typedef struct s_checker	t_checker;
typedef struct s_lexer		t_lexer;

typedef struct s_main_data
{
	t_strapi	total_input;
	t_sstr		*inputs;
	char		*partial_input;
	t_checker	*chk;
	t_lexer		*lx;
	int			*lc;
	int			lv;
	char		*stream;
	char		*ifs;
}	t_main_data;

int		read_user_input(t_main_data *data);
char	*get_heredoc_prompt(int i, int count);
bool	read_heredoc_body(char *prompt, char *delim, t_sstr **body, int lc);
bool	collect_heredocs(t_lexer *lx, int line_count, int lv);

char	*get_prompt(t_contexts *ctxs, int lv);
void	init_read(t_main_data *data);

void	save_history(t_main_data *data);
int		main_finalize(t_main_data *data);
void	init_read(t_main_data *data);
int		print_unxpected(char *s);

int		main_sigint(t_main_data *data);
bool	main_stop(t_main_data *data);
bool	main_loop(t_main_data *data);

#endif
