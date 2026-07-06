/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sstr_aux.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 21:45:17 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/12 21:11:47 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_checker.h"
#include "ft_std.h"
#include <stdlib.h>

size_t	sstrs_total_len(t_sstr *first_str)
{
	size_t	len;

	len = 0;
	while (first_str)
	{
		len += strlen(first_str->line);
		first_str = first_str->next;
	}
	return (len);
}

size_t	sstrs_count(t_sstr *first_str)
{
	size_t	count;

	count = 0;
	while (first_str)
	{
		count++;
		first_str = first_str->next;
	}
	return (count);
}
