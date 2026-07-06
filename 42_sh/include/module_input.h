/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_input.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 22:33:08 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/14 00:40:46 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODULE_INPUT_H
# define MODULE_INPUT_H

# include <stdbool.h>

typedef struct s_input
{
	char	*stream;
	int		pos;
}	t_input;

bool	peek_chr_eq(t_input *inp, char c);
char	get_char(t_input *inp);
bool	check_char(t_input *inp, char c);
bool	check_str(t_input *inp, char *str);
bool	peek_str_(t_input *inp, char *str);
void	advance_(t_input *inp);
char	peek_next_chr(t_input *inp);
bool	peek_chars_2(t_input *inp, char *str);
char	*get_ptr(t_input *inp);
bool	match_str(t_input *inp, char *str);
void	erase_and_advance_str(t_input *inp, char *str);
void	erase_and_advance(t_input *inp);

#endif
