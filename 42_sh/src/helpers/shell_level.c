/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_level.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 20:55:24 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/14 14:11:37 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "helpers.h"
#include "module_env.h"
#include <stdlib.h>

int	get_shell_level(t_mns *mns)
{
	int		shell_level;
	char	*str;

	if (mns == NULL)
		mns = get_mns(NULL);
	str = ft_getenv(&mns->env, "LW_SHLVL");
	if (str == NULL)
		return (0);
	shell_level = atoi(str);
	if (shell_level < 0)
		shell_level = 0;
	return (shell_level);
}

char	*ft_itoa_positive(int n)
{
	char	*str;
	int		len;
	int		temp;

	if (n < 0)
		return (NULL);
	len = 1;
	temp = n;
	while (temp >= 10 && len++)
		temp /= 10;
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	while (len--)
	{
		str[len] = (n % 10) + '0';
		n /= 10;
	}
	return (str);
}
