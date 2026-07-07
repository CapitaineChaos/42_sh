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

/*
 * Parseur d'options partagé des builtins (style getopt).
 * Gère les options courtes combinables (-LP), répétables, ET les options
 * longues (--posix). "--" ou le 1er non-option arrête le parsing.
 *
 * flags   : masque des options courtes vues (bit k = shorts[k]).
 * lflags  : masque des options longues vues (bit k = longs[k]).
 * last    : dernier char d'option courte vu (0 si aucun) -> "le dernier gagne".
 * count   : nombre total de chars d'option courts consommés.
 * operand : index dans argv du 1er argument non-option (== argc si aucun).
 * bad     : 1er char d'option courte invalide (0 si aucun).
 * badarg  : arg complet contenant l'option invalide (NULL si tout est valide).
 */
typedef struct s_optres
{
	unsigned int	flags;
	unsigned int	lflags;
	char			last;
	int				count;
	int				operand;
	char			bad;
	char			*badarg;
}	t_optres;

int		bt_getopt(char **argv, const char *shorts,
			const char *const *longs, t_optres *r);
bool	bt_optset(const t_optres *r, const char *shorts, char c);
bool	bt_optlong(const t_optres *r, const char *const *longs,
			const char *name);

#endif
