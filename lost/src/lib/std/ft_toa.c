/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toa.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 10:59:46 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/05 00:44:27 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_std.h"
#include <stdio.h>

char	*ft_itoa(int n)
{
	char	buf[12];

	snprintf(buf, sizeof(buf), "%d", n);
	return (strdup(buf));
}
