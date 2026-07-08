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

# ifndef L_BUFFER_SIZE
#  define L_BUFFER_SIZE 1024
# endif

# define LOG_BUF_CAP 32768

#endif
