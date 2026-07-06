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
	t_exstr			data;

	unsigned long	(*endswith)(struct s_strapi *, const char *);
	unsigned long	(*startswith)(struct s_strapi *, const char *);
	unsigned long	(*len)(struct s_strapi *);
	void			(*append_char)(struct s_strapi *, char);
	void			(*append_str)(struct s_strapi *, const char *);
	void			(*append_strn)(struct s_strapi *, const char *, size_t);
	void			(*trim_first)(struct s_strapi *, size_t);
	void			(*trim_last)(struct s_strapi *, size_t);
	char			*(*get_dup)(struct s_strapi *);
	char			*(*get_dup_n)(struct s_strapi *, size_t);
	void			(*clear)(struct s_strapi *);
	void			(*free)(struct s_strapi *);
	char			*(*get)(struct s_strapi *);
	void			(*del_last_char)(struct s_strapi *);
	void			(*del_char_at)(struct s_strapi *, size_t);
	void			(*add_char_at)(struct s_strapi *, size_t, char);
	void			(*move)(struct s_strapi *dst, struct s_strapi *src);
	int				(*expand)(struct s_exstr *, size_t);
}	t_strapi;

void	strapi_init(size_t step, t_strapi *api);

void	exstr_append_char(t_strapi *api, char c);
void	exstr_append_str(t_strapi *api, const char *s);
void	exstr_append_strn(t_strapi *api, const char *s, size_t n);
void	exstr_delete_char_at(t_strapi *api, size_t idx);
void	exstr_insert_char_at(t_strapi *api, size_t idx, char c);
int		exstr_expand(t_exstr *ex, size_t need);
size_t	exstr_length(t_strapi *api);
size_t	exstr_startswith(t_strapi *api, const char *s);
size_t	exstr_endswith(t_strapi *api, const char *s);
void	exstr_trim_first(t_strapi *api, size_t n);
void	exstr_trim_last(t_strapi *api, size_t n);
char	*exstr_get_dup(t_strapi *api);
char	*exstr_get_dup_n(t_strapi *api, size_t n);
void	exstr_free(t_strapi *api);
void	exstr_clear(t_strapi *api);
void	exstr_move(t_strapi *dst, t_strapi *src);
char	*exstr_get(t_strapi *api);
void	exstr_remove_last_char(t_strapi *api);

#endif
