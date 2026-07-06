/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_std.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 19:36:04 by smaitre           #+#    #+#             */
/*   Updated: 2025/04/29 19:43:35 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_STD_H
# define FT_STD_H

# ifndef _GNU_SOURCE
#  define _GNU_SOURCE
# endif
# include <ctype.h>
# include <string.h>
# include <stddef.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stdint.h>
# include <limits.h>
# include <unistd.h>

/**
 * STD
 */

void		free_char_array(char **argv);
char		**ft_split(char const *s, char c);
char		*ft_strjoin(char const *s1, char const *s2);

#endif
