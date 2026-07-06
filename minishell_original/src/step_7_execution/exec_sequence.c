/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_sequence.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:35:05 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 19:59:58 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_exec.h"
#include "module_parser.h"

int	exec_sequence(t_ast_node *node)
{
	int	status;
	
	if (node->left)
	{
		status = exec_node(node->left);
		set_code(get_exit_status(status));
	}
	if (node->right)
	{
		status = exec_node(node->right);
		set_code(get_exit_status(status));
	}
	return (status);
}
