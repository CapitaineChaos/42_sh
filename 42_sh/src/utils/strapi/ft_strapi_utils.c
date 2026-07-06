/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strapi_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 03:00:31 by smaitre           #+#    #+#             */
/*   Updated: 2025/04/29 20:34:28 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_strapi.h"

void	exstr_trim_last(t_strapi *api, size_t n)
{
	t_exstr	*ex;

	if (!api || n <= 0)
		return ;
	ex = &api->data;
	if (n > ex->len)
		n = ex->len;
	ex->len -= n;
	if (ex->str)
		ex->str[ex->len] = '\0';
}

size_t	exstr_endswith(t_strapi *api, const char *s)
{
	size_t	slen;
	t_exstr	*ex;

	if (!api || !s)
		return (0);
	ex = &api->data;
	slen = ft_strlen(s);
	if (slen > ex->len)
		return (0);
	return (ft_strncmp(ex->str + ex->len - slen, s, slen) == 0);
}

size_t	exstr_startswith(t_strapi *api, const char *s)
{
	t_exstr	*ex;
	size_t	slen;

	if (!api || !s)
		return (0);
	ex = &api->data;
	slen = ft_strlen(s);
	if (slen > ex->len)
		return (0);
	return (ft_strncmp(ex->str, s, slen) == 0);
}

void	exstr_trim_first(t_strapi *api, size_t n)
{
	t_exstr	*ex;

	if (!api || n <= 0)
		return ;
	ex = &api->data;
	if (n >= ex->len)
	{
		ex->len = 0;
		if (ex->str)
			ex->str[0] = '\0';
		return ;
	}
	ft_memmove(ex->str, ex->str + n, ex->len - n);
	ex->len -= n;
	ex->str[ex->len] = '\0';
}
