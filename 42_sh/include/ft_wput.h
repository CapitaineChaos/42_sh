/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wput.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 19:32:24 by smaitre           #+#    #+#             */
/*   Updated: 2025/04/29 19:34:21 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_WPUT_H
# define FT_WPUT_H

int	ft_wputchar_fd(int wc, int fd);
int	ft_wputstr_fd(char *s, int fd);

#endif