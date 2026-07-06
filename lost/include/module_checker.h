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

void	checker_init(t_checker *checker, int lv);
void	checker_reset(t_checker *checker, int lv);

void	disp_stronly(t_main_data *data, t_token *error);
void	disp_nomultiline(t_main_data *data, t_token *error);
bool	check_parens_count(t_checker *chk, int add);
void	*set_states(t_checker *chk, int rdy, int cls, int cmd);

t_token	*c_checke(t_main_data *data, t_token *cur);
t_token	*inner_checker(t_checker *chk, t_token **cur);
bool	check_lparentheses(t_checker *chk, t_token **cur, t_token **ret);
bool	check_parentheses(t_checker *chk, t_token **cur, t_token **ret);
bool	check_cmd(t_checker *chk, t_token **cur, t_token **ret);
bool	check_else(t_checker *chk, t_token **cur, t_token **ret);

bool	is_bad_redir(t_token **token);
bool	is_bad_cmd(t_token **token);
int		special_checker(t_main_data *data, t_tokens *tokens);

int		print_syntax_error(char *s, char *line);
void	rem_op_context(t_lexer *lx);
void	rem_sub_context(t_lexer *lx);
void	rem_esc_context(t_lexer *lx);
void	add_context(t_lexer *lx, t_tk_type type);

#endif
