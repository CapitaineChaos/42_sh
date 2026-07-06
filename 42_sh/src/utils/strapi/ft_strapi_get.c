/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strapi_get.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 02:58:05 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/05 21:10:24 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_strapi.h"

char	*exstr_get(t_strapi *api)
{
	if (api == NULL || api->data.str == NULL)
		return ("");
	return (api->data.str);
}

char	*exstr_get_dup(t_strapi *api)
{
	if (!api || !api->data.str)
		return (NULL);
	return (strdup(api->data.str));
}

char	*exstr_get_dup_n(t_strapi *api, size_t n)
{
	if (!api || n <= 0 || !api->data.str)
		return (NULL);
	if (n > api->data.len)
		n = api->data.len;
	return (strndup(api->data.str, n));
}

size_t	exstr_length(t_strapi *api)
{
	return (api->data.len);
}
