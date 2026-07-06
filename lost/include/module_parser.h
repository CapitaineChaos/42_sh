/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_parser.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:20:48 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/26 21:13:30 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODULE_PARSER_H
# define MODULE_PARSER_H

# include "all_config.h"
# include "module_lexer.h"
# include "module_ast.h"
# include "ft_deque.h"
# include <stddef.h>

typedef struct s_pdeques
{
	t_deque	input;
	t_deque	output;
	t_deque	final;
	t_deque	operators;
}	t_pdeques;

typedef struct s_parser
{
	struct s_pdeques	deques;
}	t_parser;

void		parser_init(t_parser *p, int lv);
void		parser_free(t_parser *p, int lv);
void		free_pdeques(t_pdeques *stacks);

bool		apply_shunting_yard(t_pdeques *deques);
void		tokens_to_nodes(t_tokens *tokens, t_deque *deque);
void		print_wildcat_error(char *str);
bool		run_parser(t_parser *prs, t_tokens *tkns, int lv);

bool		parse_word(t_tokens *tokens, t_deque *input);
bool		parse_operator(t_tokens *tokens, t_deque *input);
bool		parse_struct(t_tokens *tokens, t_deque *input);
bool		parse_control(t_tokens *tokens, t_deque *input);

size_t		get_precedence(t_ast_type type);
t_ast_type	get_ast_type(t_tk_type type);
t_ast_class	get_ast_class(t_tk_type type);

void		free_and_exit_minishell(intmax_t exit_code);
void		tk_list_append(t_tokens *tokens, t_token *tok);

#endif
