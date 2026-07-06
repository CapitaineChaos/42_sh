/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 22:52:14 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/14 00:39:28 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_FD_H
# define FT_PRINTF_FD_H

# include <stdarg.h>
# include <stddef.h>
# include <stdint.h>
# include <unistd.h>
# include "ft_printf_structs.h"

typedef struct s_env
{
	int	count;
	int	fd;
}	t_env;

typedef void	(*t_handlefct_fd)(va_list args, t_format *ft, t_env *env);

void	process_chr_fd(va_list args, t_format *ft, t_env *env);
void	process_ptr_fd(va_list args, t_format *ft, t_env *env);
void	process_str_fd(va_list args, t_format *ft, t_env *env);
void	process_nbr_fd(va_list args, t_format *ft, t_env *env);
void	process_unb_fd(va_list args, t_format *ft, t_env *env);
void	process_hex_fd(va_list args, t_format *ft, t_env *env);
void	process_pct_fd(va_list args, t_format *ft, t_env *env);

size_t	simple_str_fd(const char	*str, t_env *env);

void	apply_str_fd(const char *str, t_format *ft, t_env *env);
void	apply_hex_fd(unsigned long long nb, t_format *ft, t_env *env);
void	apply_nbr_fd(long nb, t_format *ft, t_env *env);

void	set_strparams_fd(size_t len, ssize_t precision, t_format *ft);
void	set_nbrparams_fd(int flag, t_format *ft);

int		ft_printf_fd(int fd, const char *str, ...);

#endif
