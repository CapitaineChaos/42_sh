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

	(void)mns;
	str = getenv("SHLVL");
	if (str == NULL)
		return (0);
	shell_level = atoi(str);
	if (shell_level < 0)
		shell_level = 0;
	return (shell_level);
}
