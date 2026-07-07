/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 16:40:00 by cmassol           #+#    #+#             */
/*   Updated: 2025/05/14 23:36:10 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_minishell.h"
#include "colors.h"
#include "texts.h"
#include "ft_std.h"
#include "ft_wput.h"
#include "utils.h"

void	print_part_of_edge(const char *str, int length)
{
	int	i;

	i = -1;
	while (++i < length)
		ft_wputchar_fd(str[i], 1);
}

void	print_anim2(int c_printed, int t_c_print, const char **lines, int size)
{
	int				i;
	unsigned int	chars_to_print_in_line;
	unsigned int	print_count;

	i = -1;
	while (++i < size)
	{
		chars_to_print_in_line = strlen(lines[i]);
		if (t_c_print > c_printed)
		{
			print_count = t_c_print - c_printed;
			if (print_count > chars_to_print_in_line)
			{
				print_count = chars_to_print_in_line;
			}
		}
		else
			print_count = 0;
		print_part_of_edge(lines[i], print_count);
		ft_wputchar_fd('\n', 1);
		c_printed += chars_to_print_in_line;
	}
}

void	print_animation(const char **edge_lines, int frames, int time)
{
	int	i;
	int	sizetab;
	int	c_printed;
	int	total_c_to_print;

	sizetab = 0;
	while (edge_lines[sizetab] != NULL)
		sizetab++;
	i = -1;
	ft_wputstr_fd("\033[?25l", 1);
	ft_wputstr_fd("\033[2J\033[H", 1);
	while (++i < frames)
	{
		ft_wputstr_fd("\033[0;0H", 1);
		ft_wputstr_fd(RVB7, 1);
		total_c_to_print = i;
		c_printed = 0;
		print_anim2(c_printed, total_c_to_print, edge_lines, sizetab);
		usleep(time * 1000);
	}
	ft_wputstr_fd("\033[?25h", 1);
	ft_wputstr_fd(RT, 1);
}

void	select_animation(char *name)
{
	const char	*edge_lines[] = {T1_01 T1_02, T1_03 T1_04, T1_05 T1_06,
		T1_07 T1_08, T1_09 T1_10, T1_11 T1_12, (void *)0};
	const char	*edge_lines_middle[] = {T5_01 T5_06, T5_02 T5_07,
		T5_03 T5_08, T5_04 T5_09, T5_05 T5_10, (void *)0};
	const char	*edge_lines_mini[] = {T8_01, T8_02, T8_03, (void *)0};
	const char	*edge_lines_micro[] = {T9_01, T9_02, T9_03, T9_04, T9_05,
		T9_06, T9_07, T9_08, T9_09, T9_10, T9_11, T9_12, (void *)0};

	if (strcmp(name, "gits_lines") == 0)
		print_animation(edge_lines, 1200, 1);
	else if (strcmp(name, "gits_lines_middle") == 0)
		print_animation(edge_lines_middle, 800, 2);
	else if (strcmp(name, "gits_lines_mini") == 0)
		print_animation(edge_lines_mini, 400, 6);
	else if (strcmp(name, "gits_lines_micro") == 0)
		print_animation(edge_lines_micro, 400, 7);
	else
		ft_wputstr_fd("Invalid animation name\n", 2);
}
