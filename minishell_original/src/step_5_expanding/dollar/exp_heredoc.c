/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 04:30:06 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/31 19:53:42 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_strapi.h"
#include "ft_std.h"
#include "module_expand.h"
#include "module_debug.h"

static void	append_and_advance(t_strapi *out, char c, size_t *i)
{
	out->append_char(out, c);
	(*i)++;
}

static void	process_heredoc(char *s, size_t *i, t_strapi *out)
{
	trace_info(LVL_EXPAND, "Expanding HEREDOC process");
	while (s[*i])
	{
		if (s[*i] == '\\')
		{
			if (s[*i + 1] == '\n')
			{
				*i += 2;
				continue ;
			}
			if (s[*i + 1] == '$' || s[*i + 1] == '\\' || s[*i + 1] == '`')
			{
				(*i)++;
				append_and_advance(out, s[*i], i);
				continue ;
			}
			append_and_advance(out, s[*i], i);
			continue ;
		}
		if (s[*i] == '$')
		{
			process_dollar(out, s, i);
			continue ;
		}
		append_and_advance(out, s[*i], i);
	}
}

char	*expand_heredoc(char *src, bool is_quoted)
{
	t_strapi	out;
	size_t		i;
	char		*tmp;

	if (!src)
		return (ft_strdup(""));
	if (is_quoted)
		return (ft_strdup(src));
	strapi_init(L_BUFFER_SIZE, &out);
	i = 0;
	process_heredoc(src, &i, &out);
	tmp = out.get_dup(&out);
	out.free(&out);
	return (tmp);
}

void	expand_heredoc_vars(t_tokens *tokens)
{
	t_token	*token;
	bool	has_quoted_delim;
	char	*str;
	
	if (tokens == NULL)
		return ;
	trace_info(LVL_EXPAND, "Expanding HEREDOC variables");
	token = tokens->head;
	while (token)
	{
		trace_info_nvstr(LVL_EXPAND, "Expanding HEREDOC token type", debug_get_token_type(token->type));
		trace_info_nvstr(LVL_EXPAND, "Expanding HEREDOC has dollar", token->has_dollar ? "true" : "false");
		if (token->type == TOK_HEREDOC_BODY && token->has_dollar)
		{
			has_quoted_delim = token->prev->has_quoted_part;
			trace_info_nvstr(LVL_EXPAND, "Expanding HEREDOC quoted delim", has_quoted_delim ? "true" : "false");
			str = expand_heredoc(token->str, has_quoted_delim);
			if (token->str)
				free(token->str);
			token->str = str;
			token->has_dollar = false;
		}
		token = token->next;
	}
}
