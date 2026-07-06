/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strapi_append.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 03:06:05 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/07 06:58:39 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_strapi.h"

int	exstr_expand(t_exstr *ex, size_t need)
{
	size_t	new_cap;
	char	*new;

	if (!ex->str || ex->len + need >= ex->cap)
	{
		new_cap = ex->cap;
		if (new_cap == 0)
			new_cap = ex->step;
		while (ex->len + need >= new_cap)
			new_cap += ex->step;
		new = calloc(new_cap, 1);
		if (!new)
			return (0);
		if (ex->str)
		{
			memcpy(new, ex->str, ex->len);
			free(ex->str);
		}
		new[ex->len] = '\0';
		ex->str = new;
		ex->cap = new_cap;
	}
	return (1);
}

void	exstr_append_char(t_strapi *api, char c)
{
	t_exstr	*ex;

	if (!api)
		return ;
	ex = &api->data;
	if (!exstr_expand(ex, 1))
		return ;
	ex->str[ex->len] = c;
	ex->len += 1;
	ex->str[ex->len] = '\0';
}

void	exstr_append_str(t_strapi *api, const char *s)
{
	t_exstr	*ex;
	size_t	need;

	if (!api || !s)
		return ;
	ex = &api->data;
	need = strlen(s);
	if (!exstr_expand(ex, need))
		return ;
	memcpy(ex->str + ex->len, s, need);
	ex->len += need;
	ex->str[ex->len] = '\0';
}

void	exstr_append_strn(t_strapi *api, const char *s, size_t n)
{
	t_exstr	*ex;

	if (!api || !s || n == 0)
		return ;
	ex = &api->data;
	if (!exstr_expand(ex, n))
		return ;
	memcpy(ex->str + ex->len, s, n);
	ex->len += n;
	ex->str[ex->len] = '\0';
}
