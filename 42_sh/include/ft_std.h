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

int			ft_atoi_secure(const char *nptr, bool *error);
intmax_t	ft_atoll_secure(const char *s, bool *error);
ssize_t		ft_cputchar_fd(char c, int fd);
ssize_t		ft_cputnchar_fd(char c, size_t nb, int fd);
ssize_t		ft_cputstrn_fd(char *s, size_t n, int fd);
void		free_char_array(char **argv);
char		*ft_ltoa(long n);
char		*ft_itoa(int n);
ssize_t		ft_min(ssize_t a, ssize_t b);
ssize_t		ft_max(ssize_t a, ssize_t b);
size_t		ft_nblen(long long int nb, size_t base_len);
int			get_nblen(int n);
char		**ft_split(char const *s, char c);
char		*ft_strjoin(char const *s1, char const *s2);
size_t		ft_strlcpy(char *dest, const char *src, size_t siz);
void		ft_strtoupper(char *str);
char		*ft_substr(char const *s, unsigned int start, size_t len);
size_t		ft_unblen(unsigned long long int nb, size_t base_len);
char		*ft_utoa_base(size_t nb, char *base);

#endif
