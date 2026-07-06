/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strapi_add_remove.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 23:07:27 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/14 13:54:45 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_strapi.h"
#include "ft_std.h"

void	exstr_remove_last_char(t_strapi *api)
{
	t_exstr	*ex;

	if (!api || !api->data.str || api->data.len == 0)
		return ;
	ex = &api->data;
	ex->len--;
	ex->str[ex->len] = '\0';
}

void	exstr_delete_char_at(t_strapi *api, size_t idx)
{
	t_exstr	*ex;

	ex = &api->data;
	if (ex->len == 0 || idx >= ex->len)
		return ;
	memmove(&ex->str[idx], &ex->str[idx + 1], ex->len - idx);
	ex->len--;
	ex->str[ex->len] = '\0';
}

void	exstr_insert_char_at(t_strapi *api, size_t idx, char c)
{
	t_exstr	*ex;

	ex = &api->data;
	if (idx > ex->len)
		idx = ex->len;
	if (ex->len + 2 >= ex->cap)
		exstr_expand(ex, 1);
	memmove(&ex->str[idx + 1], &ex->str[idx], ex->len - idx + 1);
	ex->str[idx] = c;
	ex->len++;
}
