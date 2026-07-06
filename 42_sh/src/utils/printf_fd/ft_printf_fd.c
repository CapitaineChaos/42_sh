/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 10:44:30 by smaitre           #+#    #+#             */
/*   Updated: 2025/04/29 20:53:03 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_fd.h"
#include "ft_std.h"

static t_handlefct_fd	get_handle(char c)
{
	if (c == 'c')
		return (process_chr_fd);
	if (c == 's')
		return (process_str_fd);
	if (c == 'p')
		return (process_ptr_fd);
	if (c == 'd' || c == 'i')
		return (process_nbr_fd);
	if (c == 'u')
		return (process_unb_fd);
	if (c == 'x' || c == 'X')
		return (process_hex_fd);
	if (c == '%')
		return (process_pct_fd);
	return (NULL);
}

static const char	*extract_flags(const char *str, t_format *format)
{
	format->flags.dash = 0;
	format->flags.zero = 0;
	format->flags.sharp = 0;
	format->flags.space = 0;
	format->flags.plus = 0;
	str++;
	while (ft_strchr("-0# +", *str))
	{
		if (*str == '-')
			format->flags.dash = 1;
		if (*str == '0')
			format->flags.zero = 1;
		if (*str == '#')
			format->flags.sharp = 1;
		if (*str == ' ')
			format->flags.space = 1;
		if (*str == '+')
			format->flags.plus = 1;
		str++;
	}
	if (format->flags.dash)
		format->flags.zero = 0;
	if (format->flags.plus)
		format->flags.space = 0;
	return (str);
}

static const char	*extract_dims(const char *str, t_format *format)
{
	format->width = 0;
	format->sign = '\0';
	format->hex = '\0';
	format->fill = ' ';
	format->precision = -1;
	if (*str == '\0')
		return (str);
	format->width = ft_atoi(str);
	while (ft_isdigit(*str))
		str++;
	if (*str == '.')
	{
		str++;
		format->precision = ft_atoi(str);
		while (ft_isdigit(*str))
			str++;
	}
	if (format->precision > 0)
		format->flags.zero = 0;
	format->argtype = *str;
	return (str);
}

static int	extract_fm(const char *str, va_list args, t_format *ft, t_env *env)
{
	const char		*suite;
	t_handlefct_fd	h_function;
	int				len;

	suite = extract_flags(str, ft);
	suite = extract_dims(suite, ft);
	suite++;
	len = (int)(suite - str);
	h_function = get_handle(ft->argtype);
	if (!ft->argtype)
		ft->argtype = 's';
	if (h_function)
		h_function(args, ft, env);
	return (len);
}

int	ft_printf_fd(int fd, const char *str, ...)
{
	va_list		args;
	t_format	format;
	t_env		env;

	if (!str)
		return (-1);
	env.count = 0;
	env.fd = fd;
	va_start(args, str);
	while (*str)
	{
		if (*str != '%')
			str += simple_str_fd(str, &env);
		else
			str += extract_fm(str, args, &format, &env);
	}
	va_end(args);
	return (env.count);
}
