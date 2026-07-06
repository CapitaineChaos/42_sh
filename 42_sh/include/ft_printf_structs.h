/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_structs.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 16:53:27 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/14 00:41:21 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_STRUCTS_H
# define FT_PRINTF_STRUCTS_H

# include <stdint.h>
# include <stddef.h>
# include <unistd.h>

typedef struct s_flags
{
	uint8_t	dash;
	uint8_t	zero;
	uint8_t	sharp;
	uint8_t	space;
	uint8_t	plus;
}	t_flags;

typedef struct s_format
{
	t_flags	flags;
	char	argtype;
	char	sign;
	char	fill;
	char	hex;
	size_t	newstr_len;
	size_t	nb_len;
	size_t	copy_len;
	size_t	width;
	size_t	nb_before;
	size_t	nb_after;
	ssize_t	precision;
}	t_format;

#endif