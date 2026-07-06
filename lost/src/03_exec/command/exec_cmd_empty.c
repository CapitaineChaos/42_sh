/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd_empty.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 02:03:59 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 20:03:18 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_exec.h"
#include "module_expand.h"
#include <stdio.h>
#include <errno.h>

int	redir_status_normalize(int r)
{
	if (r == 0)
		return (0);
	return (1);
}

int	exec_redirects_only(t_ast_node *node)
{
	int			sv_in;
	int			sv_out;
	int			ret;

	if (node->t_ast_data.operand.redirections.count == 0)
		return (EXIT_SUCCESS);
	sv_in = dup(STDIN_FILENO);
	sv_out = dup(STDOUT_FILENO);
	if (sv_in < 0 || sv_out < 0)
		return (return_code(EXIT_FAILURE, "dup"));
	ret = apply_redirs(node);
	dup2(sv_in, STDIN_FILENO);
	dup2(sv_out, STDOUT_FILENO);
	close(sv_in);
	close(sv_out);
	return (redir_status_normalize(ret));
}
