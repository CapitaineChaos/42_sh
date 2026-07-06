/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strapi.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 02:36:39 by smaitre           #+#    #+#             */
/*   Updated: 2025/04/29 19:43:44 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STRAPI_H
# define FT_STRAPI_H

# include "ft_std.h"
# include <stdlib.h>

typedef struct s_exstr
{
	char	*str;
	size_t	len;
	size_t	cap;
	size_t	step;
}	t_exstr;

typedef struct s_strapi
{
	t_exstr	data;
}	t_strapi;

void	strapi_init(size_t step, t_strapi *api);

void	exstr_append_char(t_strapi *api, char c);
void	exstr_append_str(t_strapi *api, const char *s);
int		exstr_expand(t_exstr *ex, size_t need);
char	*exstr_get_dup(t_strapi *api);
void	exstr_free(t_strapi *api);
void	exstr_clear(t_strapi *api);
char	*exstr_get(t_strapi *api);

#endif
