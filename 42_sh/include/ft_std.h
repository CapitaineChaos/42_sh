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

# include <stddef.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stdint.h>
# include <limits.h>
# include <unistd.h>

/**
 * STD
 */

int			ft_atoi(const char *nptr);
int			ft_atoi_secure(const char *nptr, bool *error);
intmax_t	ft_atoll_secure(const char *s, bool *error);
void		ft_bzero(void *s, size_t n);
void		*ft_calloc(size_t nmemb, size_t size);
ssize_t		ft_cputchar_fd(char c, int fd);
ssize_t		ft_cputnchar_fd(char c, size_t nb, int fd);
ssize_t		ft_cputstrn_fd(char *s, size_t n, int fd);
void		free_char_array(char **argv);
int			ft_isdigit(int c);
int			ft_isalpha(int c);
bool		ft_isspace(int c);
int			ft_isprint(int c);
int			ft_isalnum(int c);
char		*ft_ltoa(long n);
char		*ft_itoa(int n);
ssize_t		ft_min(ssize_t a, ssize_t b);
ssize_t		ft_max(ssize_t a, ssize_t b);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_memmove(void *dest, const void *src, size_t n);
void		*ft_memset(void *s, int c, size_t n);
size_t		ft_nblen(long long int nb, size_t base_len);
int			get_nblen(int n);
char		**ft_split(char const *s, char c);
char		*ft_strchr(const char *s, int c);
int			ft_strcmp(const char *str1, const char *str2);
char		*ft_strdup(const char *s);
void		ft_striteri(char *s, void (*f)(unsigned int, char*));
char		*ft_strjoin(char const *s1, char const *s2);
size_t		ft_strlcpy(char *dest, const char *src, size_t siz);
size_t		ft_strlen(const char *s);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strndup(const char *s, size_t n);
void		ft_strtoupper(char *str);
char		*ft_substr(char const *s, unsigned int start, size_t len);
int			ft_tolower(int c);
int			ft_toupper(int c);
size_t		ft_unblen(unsigned long long int nb, size_t base_len);
char		*ft_utoa_base(size_t nb, char *base);

#endif