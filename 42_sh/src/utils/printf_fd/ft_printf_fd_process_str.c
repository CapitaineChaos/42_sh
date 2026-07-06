/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd_process_str.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 12:24:03 by smaitre           #+#    #+#             */
/*   Updated: 2025/04/29 21:15:20 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_fd.h"
#include "ft_std.h"

void	apply_str_fd(const char *str, t_format *ft, t_env *env)
{
	env->count += ft_cputnchar_fd(' ', ft->nb_before, env->fd);
	env->count += ft_cputstrn_fd((char *)str, ft->copy_len, env->fd);
	env->count += ft_cputnchar_fd(' ', ft->nb_after, env->fd);
}

void	process_str_fd(va_list args, t_format *ft, t_env *env)
{
	char	*str;

	if (ft->precision == 0)
	{
		set_strparams_fd(0, -1, ft);
		apply_str_fd("", ft, env);
		return ;
	}
	str = (char *)va_arg(args, char *);
	if (!str)
	{
		if (ft->precision >= 6 || ft->precision == -1)
		{
			set_strparams_fd(6, 6, ft);
			apply_str_fd("(null)", ft, env);
			return ;
		}
		set_strparams_fd(0, -1, ft);
		apply_str_fd("", ft, env);
		return ;
	}
	set_strparams_fd(ft_strlen(str), ft->precision, ft);
	apply_str_fd(str, ft, env);
}

void	process_pct_fd(va_list args, t_format *ft, t_env *env)
{
	char	s[2];

	(void) args;
	s[0] = '%';
	s[1] = '\0';
	ft->width = 0;
	set_strparams_fd(1, -1, ft);
	apply_str_fd((const char *)s, ft, env);
}

void	process_chr_fd(va_list args, t_format *ft, t_env *env)
{
	char	c;
	char	s[2];

	c = (char)va_arg(args, int);
	s[0] = c;
	s[1] = '\0';
	set_strparams_fd(1, -1, ft);
	apply_str_fd((const char *)s, ft, env);
}

size_t	simple_str_fd(const char *str, t_env *env)
{
	size_t		len;
	const char	*end;

	end = str;
	while (*end && *end != '%')
		end++;
	len = (size_t)(end - str);
	env->count += ft_cputstrn_fd((char *) str, len, env->fd);
	return (len);
}
