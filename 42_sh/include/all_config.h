/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_config.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 22:33:04 by smaitre           #+#    #+#             */
/*   Updated: 2025/06/01 03:05:51 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALL_CONFIG_H
# define ALL_CONFIG_H

# ifndef _POSIX_C_SOURCE
#  define _POSIX_C_SOURCE 200809L
# endif

typedef enum e_bdg_lvl
{
	LVL_MNS = 11,
	LVL_INPUT = 10,
	LVL_CHK = 9,
	LVL_LEXER = 8,
	LVL_PARSER = 7,
	LVL_HEREDOC = 6,
	LVL_AST = 5,
	LVL_EXPAND = 4,
	LVL_ARGV = 3,
	LVL_EXEC = 2,
	LVL_FAIL = 1,
	LVL_OK = 0,
}	t_bdg_lvl;

# ifndef DLVL
#  define DLVL -2
# endif

# ifndef L_BUFFER_SIZE
#  define L_BUFFER_SIZE 1024
# endif

# define LOG_BUF_CAP 32768

#endif
