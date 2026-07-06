/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 01:19:32 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/10 01:03:36 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	free_char_array(char **argv)
{
	size_t	i;

	i = 0;
	if (!argv)
		return ;
	while (argv[i])
	{
		if (argv[i])
			free(argv[i]);
		argv[i] = NULL;
		i++;
	}
	free(argv);
}
