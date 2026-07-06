/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inp_adv.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 01:50:25 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/23 02:06:41 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_input.h"
#include "ft_std.h"

/**
 * @brief Renvoie la chaîne de caractères à partir de la position actuelle
 */
char	*get_ptr(t_input *inp)
{
	if (!inp->stream)
		return (false);
	return (inp->stream + inp->pos);
}

/**
 * @brief Avance le curseur d'un caractère sans l'écraser
 */
void	advance_(t_input *inp)
{
	if (!inp->stream)
		return ;
	if (inp->stream[inp->pos] == '\0')
		return ;
	trace_info_nvcharpos(LVL_LEXER, "Current char", inp->pos, inp->stream[inp->pos]);
	inp->pos++;
}

/**
 * @brief Remplace la portion de la chaîne par des '\0' et avance le curseur
 * @param inp Le lexer
 * @param str La chaîne à comparer
 * @note La fonction avance le curseur si la chaîne est égale
 * @note Les portions égales sont remplacées par des '\0'
 */
void	erase_and_advance_str(t_input *inp, char *str)
{
	int		i;

	if (!inp->stream)
		return ;
	i = 0;
	while (str[i] && inp->stream[inp->pos + i] == str[i])
	{
		inp->stream[inp->pos + i] = '\0';
		i++;
	}
	inp->pos += i;
}

/**
 * @brief Remplace le caractère actuel par un '\0' et avance le curseur
 * @param inp Le lexer
 * @note La fonction avance le curseur si la chaîne est égale
 */
void	erase_and_advance(t_input *inp)
{
	if (!inp->stream)
		return ;
	if (inp->stream[inp->pos] == '\0')
		return ;
	trace_info_nvcharpos(LVL_LEXER, "Current char", inp->pos, inp->stream[inp->pos]);
	inp->stream[inp->pos] = '\0';
	inp->pos++;
}
