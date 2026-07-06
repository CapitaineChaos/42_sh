/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:45:14 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 19:40:34 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "all_config.h"
# include <sys/types.h>
# include <stddef.h>
# include <stdbool.h>
# include <stdlib.h>
# include <fcntl.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 128
# endif
# define STR_MIN_SIZE 512

typedef struct s_sstr
{
	char			*line;
	struct s_sstr	*next;
}	t_sstr;

typedef struct s_logger
{
	char	buf[LOG_BUF_CAP];
	size_t	pos;
}	t_logger;

typedef struct s_stash
{
	int				fd;
	ssize_t			len;
	char			*buffer;
	struct s_stash	*next;
}	t_stash;

typedef struct s_states
{
	size_t	buffer_len;
	size_t	read_len;
	size_t	pos;
	size_t	min_len;
	int		eol;
	char	*buffer;
}	t_states;

void	log_init(t_logger *lg);
void	log_puts(t_logger *lg, const char *s);
void	log_putch(t_logger *lg, char c);
void	log_flush(int fd, t_logger *lg, bool to_file);
void	log_putlong(t_logger *lg, long n);

void	get_terminal_size(int *cols, int *rows);
void	ft_approx_sleep(int ms);

size_t	sstrs_count(t_sstr *first_str);
char	*sstrs_get_last(t_sstr *first_str);
void	sstrs_append(t_sstr **strs, char *str);
char	*sstrs_flatten_sep(t_sstr *first_str, char sep);
void	sstr_free(t_sstr **strs);
char	*sstrs_flatten(t_sstr *first_str);
size_t	sstrs_total_len(t_sstr *first_str);

char	*nano_get_next_line(int fd);

#endif
