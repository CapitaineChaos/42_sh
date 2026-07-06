/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd_process_hex.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 12:24:06 by smaitre           #+#    #+#             */
/*   Updated: 2025/04/29 21:14:56 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_fd.h"
#include "ft_std.h"
#include <stdlib.h>

void	apply_hex_fd(unsigned long long nb, t_format *ft, t_env *env)
{
	char	*tmp;
	size_t	nb_spaces_before;
	size_t	nb_spaces_after;
	size_t	nb_zeros_before;
	size_t	nb_zeros_after;

	tmp = ft_utoa_base(nb, "0123456789abcdef");
	nb_zeros_before = ft->nb_before - 2 * (ft->hex != '\0');
	nb_zeros_after = ft->nb_after;
	set_strparams_fd(ft->newstr_len, -1, ft);
	nb_spaces_before = ft->nb_before;
	nb_spaces_after = ft->nb_after;
	env->count += ft_cputnchar_fd(' ', nb_spaces_before, env->fd);
	env->count += ft_cputnchar_fd('0', (ft->hex != '\0'), env->fd);
	env->count += ft_cputnchar_fd(ft->hex, (ft->hex != '\0'), env->fd);
	env->count += ft_cputnchar_fd('0', nb_zeros_before, env->fd);
	if (ft->argtype == 'X')
		ft_strtoupper(tmp);
	env->count += ft_cputstrn_fd(tmp, ft->nb_len, env->fd);
	env->count += ft_cputnchar_fd('0', nb_zeros_after, env->fd);
	env->count += ft_cputnchar_fd(' ', nb_spaces_after, env->fd);
	free(tmp);
	return ;
}

void	process_ptr_fd(va_list args, t_format *ft, t_env *env)
{
	unsigned long long	nb;

	(void) ft;
	nb = (unsigned long long int)va_arg(args, unsigned long long int);
	if ((ft->flags.sharp || ft->argtype == 'p') && nb != 0)
		ft->hex = 'x';
	if (nb == 0)
	{
		set_strparams_fd(5, -1, ft);
		apply_str_fd("(nil)", ft, env);
		return ;
	}
	ft->nb_len = ft_unblen(nb, 16);
	set_nbrparams_fd(2 * (ft->hex != '\0'), ft);
	apply_hex_fd(nb, ft, env);
}

void	process_hex_fd(va_list args, t_format *ft, t_env *env)
{
	unsigned long long	nb;

	(void) ft;
	nb = (unsigned long long)va_arg(args, unsigned int);
	if (ft->flags.sharp && nb != 0)
	{
		ft->hex = ft->argtype;
	}
	if (nb == 0 && ft->precision == 0)
	{
		set_strparams_fd(0, ft->width, ft);
		apply_str_fd("", ft, env);
		return ;
	}
	ft->nb_len = ft_unblen(nb, 16);
	set_nbrparams_fd(2 * (ft->hex != '\0'), ft);
	apply_hex_fd(nb, ft, env);
}
