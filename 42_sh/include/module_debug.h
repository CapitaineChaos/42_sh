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


void	debug_lx_pendings(t_lexer *lx);
void	debug_contexts(t_contexts *ctxs);
void	debug_tokens(t_tokens *tokens, int lv);
void	debug_str_array(char *name, char **argv);
void	debug_ast_tree(t_deque *stack);
void	debug_ast_node(t_ast_node *node, int indent);
void	debug_chk_pendings(t_checker *chk, char *msg);
void	debug_title(int LVL, char *title);
void	debug_mini_title(int LVL, char *title);
void	debug_nano_title(int LVL, char *title);
void	debug_pico_title(int LVL, char *title, char *color);
void	debug_argv(char *name, char **argv);
void	debug_tokens_list_str(t_token *list);
void	debug_tokens_list_stream(t_token *list);
void	debug_simple_token_list_all_nodes(t_ast_node *first);

void	get_token_content_str(t_token *token, char *buf, size_t buf_size);
void	get_token_content_stream(t_token *token, char *buf, size_t buf_size);
void	get_tkpart_content_str(t_tk_part *part, char *buf, size_t buf_size);
void	get_tkpart_content_stream(t_tk_part *part, char *buf, size_t buf_size);
void	debug_log(const char *msg);
void	debug_log_init(void);
void	trace_logger_flush(int fd, t_logger *lg, bool to_file);

void	dbg_reset(void);
void	dbg_read(t_sstr *inputs);
void	dbg_tokens(t_tokens *tokens);
void	dbg_nodes(t_ast_node *root);
void	dbg_ast(t_deque *final);
void	dbg_exec(t_ast_node *root);
void	dbg_footer(int last_exit);

char	*get_ast_typestr(t_ast_type type);
char	*debug_get_token_type(t_tk_type type);
char	*debug_get_tok_kind(t_tk_kind kind);
char	*get_tok_family(t_tk_family tclass);
char	*convert_dbg_lvl_name(int lvl);
void	debug_ast_node_infos(t_ast_node *node, char *sender);
void	trace_ok(int lvl, char *str);
void	trace_ko(int lvl, char *str);
void	trace_start(int lvl, char *str);
void	trace_fatal(int lvl, char *str);
void	trace_info(int lvl, char *str);
void	trace_debug_level(void);
void	trace_short_info(int lv, char *str);
void	trace_info_nvstr(int lv, char *name, char *value);
void	trace_info_nvchr(int lv, char *name, char c);
void	trace_info_nvnb(int lv, char *name, int value);
void	trace_short_info_no_nl(int lv, char *str);
void	debug_escape_print(char *s);

void	trace_info_nvcharpos(int lv, char *name, int pos, char c);

#endif
