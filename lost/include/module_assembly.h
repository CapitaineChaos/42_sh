/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_assembly.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 03:57:07 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/24 07:03:25 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODULE_ASSEMBLY_H
# define MODULE_ASSEMBLY_H

# include <stdbool.h>
# include <stddef.h>
# include "module_token.h"
# include "module_ast.h"
# include "module_minishell.h"

char	**create_cmd_argv(t_tokens *tokens, int *argc);
int		assemble_node(t_ast_node *node);

#endif
