/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strapi_lifecycle.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 02:36:24 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/07 23:42:39 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_strapi.h"

void	exstr_move(t_strapi *dst, t_strapi *src)
{
	dst->data.str = src->data.str;
	dst->data.len = src->data.len;
	dst->data.cap = src->data.cap;
	dst->data.step = src->data.step;
	src->data.str = NULL;
	src->data.len = 0;
	src->data.cap = 0;
}

void	strapi_init(size_t step, t_strapi *api)
{
	ft_memset(api, 0, sizeof(t_strapi));
	api->data.step = step;
	api->append_str = &exstr_append_str;
	api->append_strn = &exstr_append_strn;
	api->append_char = &exstr_append_char;
	api->clear = &exstr_clear;
	api->free = &exstr_free;
	api->get = &exstr_get;
	api->get_dup = &exstr_get_dup;
	api->get_dup_n = &exstr_get_dup_n;
	api->len = &exstr_length;
	api->startswith = &exstr_startswith;
	api->endswith = &exstr_endswith;
	api->trim_first = &exstr_trim_first;
	api->trim_last = &exstr_trim_last;
	api->del_last_char = &exstr_remove_last_char;
	api->del_char_at = &exstr_delete_char_at;
	api->add_char_at = &exstr_insert_char_at;
	api->expand = &exstr_expand;
}

void	exstr_clear(t_strapi *api)
{
	t_exstr	*ex;

	ex = &api->data;
	ex->len = 0;
	if (ex->str)
		ft_bzero(ex->str, ex->cap);
}

void	exstr_free(t_strapi *api)
{
	t_exstr	*ex;

	ex = &api->data;
	if (ex->str)
		free(ex->str);
	ex->str = NULL;
	ex->len = 0;
	ex->cap = 0;
}
