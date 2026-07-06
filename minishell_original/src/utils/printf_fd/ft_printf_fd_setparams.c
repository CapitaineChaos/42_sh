/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd_setparams.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 03:00:38 by profchaos         #+#    #+#             */
/*   Updated: 2025/04/29 20:58:53 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_fd.h"
#include "ft_std.h"

void	set_nbrparams_fd(int flag, t_format *ft)
{
	size_t	padding_len;

	padding_len = 0;
	if (ft->precision > 0)
		padding_len = ft->precision;
	if (ft->flags.zero && ft->precision == -1)
		padding_len = ft->width - flag;
	ft->newstr_len = ft_max(ft->nb_len, padding_len) + flag;
	ft->nb_before = ft->newstr_len - ft->nb_len;
	ft->nb_after = ft->newstr_len - ft->nb_len - ft->nb_before;
}

void	set_strparams_fd(size_t len, ssize_t precision, t_format *ft)
{
	ft->copy_len = len;
	if (precision >= 0)
		ft->copy_len = ft_min(ft->copy_len, precision);
	ft->newstr_len = ft_max(ft->copy_len, ft->width);
	ft->nb_before = ft->newstr_len - ft->copy_len;
	if (ft->flags.dash)
		ft->nb_before = 0;
	ft->nb_after = ft->newstr_len - ft->copy_len - ft->nb_before;
}
