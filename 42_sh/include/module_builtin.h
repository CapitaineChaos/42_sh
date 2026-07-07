/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_builtin.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 22:53:25 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/23 02:45:35 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODULE_BUILTIN_H
# define MODULE_BUILTIN_H

# include "module_minishell.h"
# include "all_config.h"

typedef int	(*t_builtin_fct)(t_mns *mns, int argc, char **argv, char **envp);

typedef struct s_builtin_entry
{
	const char		*name;
	t_builtin_fct	fn;
}	t_builtin_entry;

int	builtin_dot(t_mns *mns, int argc, char **argv, char **envp);
int	builtin_cd(t_mns *mns, int argc, char **argv, char **envp);
int	builtin_pwd(t_mns *mns, int argc, char **argv, char **envp);
int	builtin_echo(t_mns *mns, int argc, char **argv, char **envp);
int	builtin_exit(t_mns *mns, int argc, char **argv, char **envp);
int	builtin_export(t_mns *mns, int argc, char **argv, char **envp);
int	builtin_unset(t_mns *mns, int argc, char **argv, char **envp);

#endif
