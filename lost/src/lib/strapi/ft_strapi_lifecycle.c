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

void	strapi_init(size_t step, t_strapi *api)
{
	memset(api, 0, sizeof(t_strapi));
	api->data.step = step;
}

void	exstr_clear(t_strapi *api)
{
	t_exstr	*ex;

	ex = &api->data;
	ex->len = 0;
	if (ex->str)
		memset(ex->str, 0, ex->cap);
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
