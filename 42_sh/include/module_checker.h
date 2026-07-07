/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_checker.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 22:56:44 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 00:35:43 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODULE_CHECKER_H
# define MODULE_CHECKER_H

# include "module_context.h"
# include "module_lexer.h"
# include "module_input.h"
# include "module_read.h"
# include "module_token.h"
# include "utils.h"

typedef struct s_tokens		t_tokens;
typedef struct s_token		t_token;
typedef struct s_tk_part	t_tk_part;
typedef struct s_main_data	t_main_data;

typedef struct s_checker
{
	bool		can_cmd;
	bool		cmd_closed;
	bool		cmd_ready;


	bool		pending_escape;
	int			parentheses;
	t_token		*current;
	bool		proceed_loop;
	t_input		inp;
	char		*ifs;
}	t_checker;

/**
 * LIFECYCLE
 */

void	checker_init(t_checker *checker);
void	checker_reset(t_checker *checker);

/**
 * MAIN
 */

int		special_checker(t_main_data *data, t_tokens *tokens);

/**
 * SHORTCUTS
 */

int		print_syntax_error(char *s, char *line);
void	rem_op_context(t_lexer *lx);
void	rem_sub_context(t_lexer *lx);
void	rem_esc_context(t_lexer *lx);
void	add_context(t_lexer *lx, t_tk_type type);

#endif
