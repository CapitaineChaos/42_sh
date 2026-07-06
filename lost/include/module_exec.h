/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_exec.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 01:40:28 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 02:33:26 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODULE_EXEC_H
# define MODULE_EXEC_H

# include <stdbool.h>
# include "module_minishell.h"
# include "module_ast.h"

bool	run_exec(t_ast_node *first, int lv);
int		exec_node(t_ast_node *node);
int		exec_and_or(t_ast_node *node);
int		exec_sequence(t_ast_node *node);
int		exec_subshell(t_ast_node *node);
int		apply_redirs(t_ast_node *node);

bool	is_directory(const char *path);
bool	is_builtin(char *cmd);
int		exec_command(t_ast_node *node);
int		exec_external(t_ast_node *node, char *path, char **av, char **ep);
int		execute_builtin(int argc, char **argv);
int		exec_builtin(t_ast_node *node, char **argv, t_operand *op);
int		exec_redirects_only(t_ast_node *node);

int		wait_for_children(t_ast_node **cmds, int ncmds);
int		exec_pipeline(t_ast_node *node);
int		collect_pipeline_cmds(t_ast_node *node, t_ast_node **tab, int idx);
int		get_pipeline_cmds_count(t_ast_node *node, int *count);

int		disp_msg_and_ret(char *msg, int ret);
int		display_other_msg(int sig, bool next_is_pipe);
void	display_msg_err(char *cmd, int err);
void	display_cmd_err(char *cmd);

int		set_code(int exit_code);
int		return_code(int ret, char *str);
int		return_full_code(int status);
int		get_exit_status(intmax_t exit_status);
int		get_exit_code(char *cmd, int err);

bool	is_directory(const char *path);
int		write_buffer(int fd, t_logger *lg);
bool	has_slash(const char *s);
char	*get_exec_path(t_mns *mns, char **argv);

#endif
