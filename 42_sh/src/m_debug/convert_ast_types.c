/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_ast_types.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:16:19 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 00:04:45 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_ast.h"

char	*get_ast_typestr(t_ast_type type)
{
	if (type == AST_CMD)
		return ("CMD");
	else if (type == AST_PIPE)
		return ("PIPE");
	else if (type == AST_AND)
		return ("AND");
	else if (type == AST_OR)
		return ("OR");
	else if (type == AST_HEREDOC)
		return ("HEREDOC");
	else if (type == AST_SUBSHELL)
		return ("SUBSHELL");
	else if (type == AST_SUBSH_MARK)
		return ("SUBSH_MARK");
	else if (type == AST_SUBSH_END)
		return ("SUBSH_END");
	else if (type == AST_SEQ)
		return ("SEQUENCE");
	else
		return ("UNKNOWN");
}
