/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_env.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 22:52:38 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 21:03:53 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODULE_ENV_H
# define MODULE_ENV_H

# include <stdbool.h>

struct					s_mns;
typedef struct s_mns	t_mns;

typedef struct s_env_node
{
	char				*key;
	char				*value;
	bool				hidden;
	struct s_env_node	*prev;
	struct s_env_node	*next;
}	t_env_node;

typedef struct s_env_lst
{
	t_env_node	*head;
	t_env_node	*tail;
	int			size;
}	t_env_lst;

void		create_env(t_mns *mns);
char		*ft_getenv(t_env_lst *env, char *name);
void		free_env(t_mns *mns);

/**
 * ENV LIST
 */

void		env_list_set(t_env_lst *lst, const char *key, const char *value);
void		hide_node_key(t_env_lst *lst, const char *key);
char		**env_list_to_envp(t_env_lst *lst);
void		envp_to_env_list(t_env_lst *lst, char **envp);

/**
 * LIFECYCLE
 */

void		env_list_free(t_env_lst *lst);
void		env_list_init(t_env_lst *lst, char **envp);
void		env_list_unset(t_env_lst *lst, const char *key);

/**
 * OPS
 */

void		env_list_rem_node(t_env_lst *lst, t_env_node *node);
t_env_node	*env_list_create_node(const char *k, const char *value);
void		env_lst_add_entry(t_env_lst *lst, const char *k, const char *v);
void		env_lst_add_entry_hid(t_env_lst *lst, const char *k, const char *v);

/**
 * SORTING
 */

void		env_lst_reorder(t_env_lst *lst);

#endif