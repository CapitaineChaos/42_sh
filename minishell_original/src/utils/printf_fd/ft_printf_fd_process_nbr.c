/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd_process_nbr.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 12:24:06 by smaitre           #+#    #+#             */
/*   Updated: 2025/04/29 21:14:53 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_fd.h"
#include "ft_std.h"
#include <stdlib.h>

void	apply_nbr_fd(long nb, t_format *ft, t_env *env)
{
	char	*tmp;
	size_t	nb_spaces_before;
	size_t	nb_spaces_after;
	size_t	nb_zeros_before;
	size_t	nb_zeros_after;

	if (nb < 0)
		nb = -nb;
	tmp = ft_ltoa(nb);
	nb_zeros_before = ft->nb_before - (int)(ft->sign != '\0');
	nb_zeros_after = ft->nb_after;
	set_strparams_fd(ft->newstr_len, -1, ft);
	nb_spaces_before = ft->nb_before;
	nb_spaces_after = ft->nb_after;
	env->count += ft_cputnchar_fd(' ', nb_spaces_before, env->fd);
	env->count += ft_cputnchar_fd(ft->sign, (int)(ft->sign != '\0'), env->fd);
	env->count += ft_cputnchar_fd('0', nb_zeros_before, env->fd);
	env->count += ft_cputstrn_fd(tmp, ft->nb_len, env->fd);
	env->count += ft_cputnchar_fd('0', nb_zeros_after, env->fd);
	env->count += ft_cputnchar_fd(' ', nb_spaces_after, env->fd);
	free(tmp);
	return ;
}

void	process_unb_fd(va_list args, t_format *ft, t_env *env)
{
	long	nb;

	(void) ft;
	nb = (long)va_arg(args, unsigned int);
	if (nb == 0 && ft->precision == 0)
	{
		set_strparams_fd(0, ft->width, ft);
		apply_str_fd((char *){&ft->sign}, ft, env);
		return ;
	}
	ft->nb_len = ft_unblen(nb, 10);
	set_nbrparams_fd(0, ft);
	apply_nbr_fd(nb, ft, env);
}

void	process_nbr_fd(va_list args, t_format *ft, t_env *env)
{
	long	nb;
	int		flag;

	(void) ft;
	nb = (long)va_arg(args, int);
	if (nb < 0)
		ft->sign = '-';
	if (nb >= 0 && ft->flags.plus)
		ft->sign = '+';
	if (nb >= 0 && ft->flags.space)
		ft->sign = ' ';
	flag = (int)(ft->sign != '\0');
	if (nb == 0 && ft->precision == 0)
	{
		set_strparams_fd(flag, ft->width, ft);
		apply_str_fd((char *){&ft->sign}, ft, env);
		return ;
	}
	ft->nb_len = ft_nblen(nb, 10);
	if (nb < 0)
		ft->nb_len -= 1;
	set_nbrparams_fd(flag, ft);
	apply_nbr_fd(nb, ft, env);
}
