/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_var.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 04:11:09 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/27 06:50:00 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_config.h"
#include "ft_std.h"
#include "module_expand.h"
#include "module_env.h"
#include "helpers.h"

char	*fetch_normal_var(char *s, size_t *i)
{
	char	*key;
	char	*val;

	key = get_identifier(s, i);
	if (key == NULL)
		return (NULL);
	val = ft_getenv(&g_shell->env, key);
	free(key);
	if (val == NULL)
		return ("");
	return (val);
}

bool	process_dollar(t_strapi *out, char *s, size_t *i)
{
	char	*tmp;

	if (s[*i] == '$' && is_unescaped_delim(s, *i, "$"))
	{
		(*i)++;
		tmp = fetch_special_var(s, i);
		if (tmp)
		{
			exstr_append_str(out, tmp);
			free(tmp);
			return (true);
		}
		tmp = fetch_normal_var(s, i);
		if (tmp)
		{
			exstr_append_str(out, tmp);
			return (true);
		}
		exstr_append_char(out, '$');
		return (true);
	}
	exstr_append_char(out, s[*i]);
	(*i)++;
	return (false);
}

char	*expand_dollar(char *src)
{
	t_strapi	out;
	size_t		i;
	char		*tmp;

	if (!src)
		return (strdup(""));
	strapi_init(L_BUFFER_SIZE, &out);
	i = 0;
	while (src[i])
	{
		if (src[i] != '$')
		{
			exstr_append_char(&out, src[i]);
			i++;
			continue ;
		}
		process_dollar(&out, src, &i);
	}
	tmp = exstr_get_dup(&out);
	exstr_free(&out);
	return (tmp);
}

void	expand_redir_vars(t_tokens *tokens)
{
	t_token		*token;
	t_tk_part	*part;

	if (tokens == NULL)
		return ;
	token = tokens->head;
	while (token)
	{
		if (token->has_dollar)
		{
			part = token->head;
			while (part)
			{
				if (part->has_dollar)
					repl_and_free_str(part, expand_dollar(part->str));
				part->has_dollar = false;
				part = part->next;
			}
		}
		token = token->next;
	}
}

void	expand_cmd_vars(t_tokens *tokens)
{
	t_token		*token;
	t_tk_part	*part;

	if (tokens == NULL)
		return ;
	token = tokens->head;
	while (token)
	{
		if (token->has_dollar)
		{
			part = token->head;
			while (part)
			{
				if (part->has_dollar)
					repl_and_free_str(part, expand_dollar(part->str));
				part->has_dollar = false;
				part = part->next;
			}
		}
		token = token->next;
	}
}
