/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 13:27:14 by smaitre           #+#    #+#             */
/*   Updated: 2025/04/29 20:41:01 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_std.h"
#include <stdlib.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*str;

	if (!s1 && !s2)
		return (strdup(""));
	if (!s1)
		return (strdup(s2));
	if (!s2)
		return (strdup(s1));
	len_s1 = strlen(s1);
	len_s2 = strlen(s2);
	str = (char *)malloc((len_s1 + len_s2 + 1) * sizeof (char));
	if (!str)
		return (NULL);
	memcpy(str, s1, len_s1);
	memcpy(str + len_s1, s2, len_s2);
	str[len_s1 + len_s2] = '\0';
	return (str);
}
