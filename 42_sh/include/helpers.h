/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 20:56:28 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/23 02:00:03 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPERS_H
# define HELPERS_H

# include "module_debug.h"
# include "module_minishell.h"

int		get_shell_level(t_mns *mns);
char	*ft_itoa_positive(int n);
bool	is_valid_id(const char *s);
char	*get_identifier(const char *str, size_t *idx);

/**
 * PROC
 */

int		get_uid_from_proc(void);
int		get_fd_soft_limit_from_proc(void);

#endif