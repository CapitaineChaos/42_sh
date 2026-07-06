/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 04:30:58 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 21:15:41 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_expand.h"
#include "module_exec.h"
#include "ft_strapi.h"
#include "utils.h"
#include <stdlib.h>

bool	repl_and_free_str(t_tk_part *part, char *new_str)
{
	if (new_str == NULL)
		return (false);
	if (part->str == NULL)
	{
		part->str = new_str;
		return (true);
	}
	free(part->str);
	part->str = new_str;
	return (true);
}

void	copy_char(t_strapi *out, char *s, size_t *i)
{
	exstr_append_char(out, s[*i]);
	(*i)++;
}

void	*return_and_free(t_strapi *out)
{
	char	*result;

	result = exstr_get_dup(out);
	exstr_free(out);
	return (result);
}

int	return_and_send_error(int n)
{
	t_logger	logger;

	log_init(&logger);
	if (n == 0)
	{
		log_puts(&logger, "🐯: ambiguous redirect\n");
	}
	if (n > 1)
	{
		log_puts(&logger, "🐯: ambiguous redirect\n");
	}
	log_flush(STDERR_FILENO, &logger, false);
	return (-1);
}

int	find_unescaped_char(char *s, char c)
{
	int	i;
	int	bs;
	int	j;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
		{
			bs = 0;
			j = i - 1;
			while (j >= 0 && s[j] == '\\')
			{
				bs++;
				j--;
			}
			if (bs % 2 == 0)
				return (i);
		}
		i++;
	}
	return (-1);
}
