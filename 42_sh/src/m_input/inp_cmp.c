/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inp_cmp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 02:51:47 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/17 01:43:00 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_input.h"
#include "ft_std.h"

/**
 * @brief Compare le caractère actuel avec le caractère passé en paramètre
 * @param lx Le lexer
 * @param c Le caractère à comparer
 */
bool	peek_chr_eq(t_input *inp, char c)
{
	if (!inp->stream)
		return (false);
	if (inp->stream[inp->pos] != c)
		return (false);
	return (true);
}

/**
 * @brief Retourne le caractère à la position en cours sans avancer le curseur
 * @param inp Le curseur
 * @return Le caractère à la position en cours
 */
char	get_char(t_input *inp)
{
	if (!inp->stream)
		return (false);
	return (inp->stream[inp->pos]);
}

/**
 * @brief Compare le caractère et avance le curseur si c'est le bon
 * @param inp Le curseur
 * @param c Le caractère à comparer
 * @return true si le caractère est le bon, false sinon
 * @note N'écrase pas le caractère
 */
bool	check_char(t_input *inp, char c)
{
	if (!inp->stream)
		return (false);
	if (inp->stream[inp->pos] == c)
	{
		advance_(inp);
		return (true);
	}
	return (false);
}

/**
 * @brief Compare la chaîne de caractères et avance le curseur si c'est ok
 * @param inp Le curseur
 * @param str La chaîne de caractères à comparer
 * @return true si la chaîne de caractères est la bonne, false sinon
 */
bool	check_str(t_input *inp, char *str)
{
	size_t	i;

	if (!inp->stream)
		return (false);
	i = 0;
	while (str[i] && inp->stream[inp->pos + i] == str[i])
		i++;
	if (str[i] != '\0')
		return (false);
	inp->pos += i;
	return (true);
}

/**
 * @brief Compare la chaîne de caractères et ne fait rien si c'est ok
 * @param inp Le curseur
 * @param str La chaîne de caractères à comparer
 * @return true si la chaîne de caractères est la bonne, false sinon
 */
bool	peek_str_(t_input *inp, char *str)
{
	int		i;

	if (!inp->stream)
		return (false);
	i = 0;
	while (str[i] && inp->stream[inp->pos + i] == str[i])
		i++;
	if (str[i] != '\0')
		return (false);
	return (true);
}
