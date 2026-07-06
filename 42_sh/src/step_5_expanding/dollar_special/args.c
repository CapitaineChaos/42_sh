/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 00:50:09 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/25 03:23:14 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "all_config.h"
#include "ft_std.h"
#include "module_env.h"
#include "module_expand.h"
#include "module_minishell.h"
#include "helpers.h"

static char	*join_args(t_mns *mns)
{
	t_strapi	out;
	int			idx;

	strapi_init(L_BUFFER_SIZE, &out);
	idx = 1;
	while (idx < mns->argc)
	{
		out.append_str(&out, mns->argv[idx]);
		if (idx + 1 < mns->argc)
			out.append_char(&out, ' ');
		idx++;
	}
	return (return_and_free(&out));
}

static char	*get_exit_status(size_t *i)
{
	char	*val;

	debug_nano_title(LVL_EXPAND, "Processing exit status");
	val = ft_itoa(get_mns(NULL)->last_exit_code);
	trace_info_nvnb(LVL_EXPAND, "Exit status", get_mns(NULL)->last_exit_code);
	(*i)++;
	return (val);
}

static char	*get_numeric_arg(size_t *i, char *s)
{
	t_mns	*mns;
	char	*val;
	int		idx;

	debug_nano_title(LVL_EXPAND, "Processing numeric arg");
	idx = s[*i] - '0';
	(*i)++;
	mns = get_mns(NULL);
	if (mns->argc > idx)
		val = strdup(mns->argv[idx]);
	else
		val = strdup("");
	return (val);
}

static char	*get_positional_args(void)
{
	t_mns	*mns;

	debug_nano_title(LVL_EXPAND, "Processing positional args");
	mns = get_mns(NULL);
	if (mns->argc > 1)
		return (join_args(mns));
	return (strdup(""));
}

char	*fetch_special_var(char *s, size_t *i)
{
	if (s[*i] == '?')
		return (get_exit_status(i));
	if (isdigit((unsigned char)s[*i]))
		return (get_numeric_arg(i, s));
	if (s[*i] == '*' || s[*i] == '@')
	{
		s[*i] = '\0';
		(*i)++;
		return (get_positional_args());
	}
	return (NULL);
}
