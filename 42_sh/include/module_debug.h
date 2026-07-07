/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_debug.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 20:01:22 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 03:57:13 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODULE_DEBUG_H
# define MODULE_DEBUG_H

# include "all_config.h"
# include "module_token.h"
# include "module_ast.h"

typedef struct s_logger t_logger;
typedef struct s_contexts t_contexts;	
typedef struct s_tokens t_tokens;
typedef struct s_deque t_deque;
typedef struct s_ast_node t_ast_node;
typedef struct s_checker t_checker;
typedef struct s_parser t_parser;
typedef struct s_sstr t_sstr;


void	get_token_content_stream(t_token *token, char *buf, size_t buf_size);
void	get_tkpart_content_stream(t_tk_part *part, char *buf, size_t buf_size);

void	dbg_reset(void);
void	dbg_read(t_sstr *inputs);
void	dbg_tokens(t_tokens *tokens);
void	dbg_nodes(t_ast_node *root);
void	dbg_ast(t_deque *final);
void	dbg_exec(t_ast_node *root);
void	dbg_footer(int last_exit);
void	dbg_error(const char *msg);
void	dbg_errors_reset(void);
void	dbg_logf(const char *fmt, ...);
void	dbg_assert_fail(const char *expr, const char *msg,
			const char *file, int line);

/*
 * Panel error du debugview = canal d'assertions / logs de dev.
 * DBG_ASSERT(cond, msg) : émet une entrée si cond est fausse.
 * DBG_LOGF(fmt, ...)     : log formaté (ex: DBG_LOGF("count=%d", n)).
 * No-op total hors build debug (make debug -> -DDBG_VIEW).
 */
# ifdef DBG_VIEW
#  define DBG_ASSERT(cond, msg) ((cond) ? (void)0 \
	: dbg_assert_fail(#cond, (msg), __FILE__, __LINE__))
#  define DBG_LOGF(...) dbg_logf(__VA_ARGS__)
# else
#  define DBG_ASSERT(cond, msg) ((void)0)
#  define DBG_LOGF(...) ((void)0)
# endif

char	*get_ast_typestr(t_ast_type type);
char	*debug_get_token_type(t_tk_type type);
char	*debug_get_tok_kind(t_tk_kind kind);
char	*get_tok_family(t_tk_family tclass);
char	*convert_dbg_lvl_name(int lvl);

#endif
