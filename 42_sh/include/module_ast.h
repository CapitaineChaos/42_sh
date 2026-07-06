/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_ast.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 16:58:43 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 00:04:51 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODULE_AST_H
# define MODULE_AST_H

# include "ft_deque.h"
# include "module_token.h"

typedef enum e_ast_type
{
	AST_CMD,
	AST_PIPE,
	AST_AND,
	AST_OR,
	AST_HEREDOC,
	AST_SUBSHELL,
	AST_SUBSH_MARK,
	AST_SUBSH_END,
	AST_SEQ,
	AST_UNKNOWN,
}	t_ast_type;

typedef enum e_ast_class
{
	ACL_OPERATOR,
	ACL_OPERAND,
	ACL_STRUCT,
	ACL_CONTROL,
}	t_ast_class;

typedef struct s_operator
{
	size_t		precedence;
	t_token		*token;
}	t_operator;

typedef struct s_operand
{
	t_tokens	tokens;
	t_tokens	redirections;
	char		**argv;
	char		**envp;
	char		*path;
	int			argc;
}	t_operand;

typedef struct s_ast_node
{
	t_ast_type			type;
	t_ast_class			tclass;
	int					arity;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	struct s_ast_node	*parent;
	int					id;
	int					pid;
	int					status;
	int					exit_code;
	union u_ast_data
	{
		t_operator		operator_;
		t_operand		operand;
	}	t_ast_data;
}	t_ast_node;

/**
 * AST LIFECYCLE
 */

void		free_ast(t_ast_node *node);
void		free_ast_node(t_ast_node *node);

/**
 * CREATE
 */

t_ast_node	*create_cmd_word_operand(t_ast_type type);
t_ast_node	*create_operator(t_ast_type type, t_token *token);
t_ast_node	*create_control(t_ast_type type, t_token *token);

/**
 * HELPERS
 */

t_ast_node	*peek_head(t_deque *deque);

/**
 * PUSH_SWAP FUNCTIONS
 */

void		ps_push_front_create(t_deque *deque, t_ast_node *token);

#endif