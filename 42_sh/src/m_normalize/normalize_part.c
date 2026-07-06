/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_part.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 18:20:05 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 19:44:33 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_config.h"
#include "ft_strapi.h"
#include "ft_std.h"
#include "module_expand.h"
#include "module_normalize.h"

static void	init_format(t_strapi *out, size_t *i)
{
	strapi_init(L_BUFFER_SIZE, out);
	out->append_str(out, "");
	*i = 0;
}

static char	*format_dquoted(char *str)
{
	t_strapi	out;
	size_t		i;
	char		c;

	init_format(&out, &i);
	while (str[i])
	{
		if (str[i] == '\\')
		{
			c = str[i + 1];
			if (c == '$' || c == '"' || c == '\\' || c == '`')
			{
				i++;
				copy_char(&out, str, &i);
			}
			else if (c == '\n')
				i += 2;
			else
				copy_char(&out, str, &i);
			continue ;
		}
		copy_char(&out, str, &i);
	}
	return (return_and_free(&out));
}

static char	*format_unquoted(char *str)
{
	t_strapi	out;
	size_t		i;

	init_format(&out, &i);
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1] != '\0')
		{
			i++;
			copy_char(&out, str, &i);
			continue ;
		}
		copy_char(&out, str, &i);		
	}
	return (return_and_free(&out));
}

char	*normalize_part(t_tk_part *part)
{
	char	*str;

	if (part->str && part->type == TOK_SQUOTE)
		return (strdup(part->str));
	else if (part->str && part->type == TOK_DQUOTE)
		return (format_dquoted(part->str));
	else if (part->str && part->type == TOK_UQUOTE)
	{
		str = format_unquoted(part->str);
		if (str && str[0] == '\0')
		{
			free(str);
			return (NULL);
		}
		return (str);
	}
	return (NULL);
}

void	normalize_parts(t_tk_part *first)
{
	t_tk_part	*part;
	char		*frag;

	trace_info(LVL_EXPAND, "Formatting parts");
	if (first == NULL)
		return ;
	part = first;
	while (part)
	{
		frag = normalize_part(part);
		if (frag)
		{
			free(part->str);
			part->str = frag;
		}
		part = part->next;
	}
	trace_info_nvstr(LVL_EXPAND, "Token type        ", debug_get_token_type(first->type));
	trace_info_nvstr(LVL_EXPAND, "Token part        ", frag);
}
