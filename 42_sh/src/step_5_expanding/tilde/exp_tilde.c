/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_tilde.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 17:41:32 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/25 21:24:02 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_std.h"
#include "module_env.h"
#include "module_expand.h"
#include "module_token.h"
#include "helpers.h"
#include <pwd.h>

static char	*expand_pwd_variant(const char *w)
{
	const char	*base;

	if (w[1] != '+' && w[1] != '-')
		return (NULL);
	if (w[2] != '\0' && w[2] != '/')
		return (NULL);
	if (w[1] == '+')
		base = getenv("PWD");
	else
		base = getenv("OLDPWD");
	if (!base || !*base)
		return (NULL);
	if (w[2] == '\0')
		return (strdup(base));
	return (ft_strjoin(base, w + 2));
}

static char	*expand_home_variant(const char *w)
{
	const char	*base;

	if (w[1] != '\0' && w[1] != '/')
		return (NULL);
	base = getenv("HOME");
	if (!base || !*base)
		return (NULL);
	if (w[1] == '\0')
		return (strdup(base));
	return (ft_strjoin(base, w + 1));
}

static char	*expand_user_variant(const char *w)
{
	struct passwd	*pw;
	char			*login;
	const char		*slash;
	char			*res;

	if (w[1] == '\0' || w[1] == '/' || w[1] == '+' || w[1] == '-')
		return (NULL);
	slash = strchr(w, '/');
	if (slash)
		login = strndup(w + 1, slash - (w + 1));
	else
		login = strdup(w + 1);
	if (!login)
		return (NULL);
	pw = getpwnam(login);
	free(login);
	if (!pw || !pw->pw_dir)
		return (NULL);
	if (!slash)
		return (strdup(pw->pw_dir));
	res = ft_strjoin(pw->pw_dir, slash);
	return (res);
}

char	*tilde_expand_word(const char *w)
{
	char	*res;

	if (w[0] != '~')
		return (NULL);
	res = expand_pwd_variant(w);
	if (res)
		return (res);
	res = expand_home_variant(w);
	if (res)
		return (res);
	return (expand_user_variant(w));
}

int	expand_cmd_tildes(t_tokens *tokens)
{
	t_token	*tok;
	char	*expanded;

	if (tokens == NULL)
		return (0);
	tok = tokens->head;
	while (tok)
	{
		if (tok->has_tilde)
		{
			expanded = tilde_expand_word(tok->head->str);
			if (expanded)
				repl_and_free_str(tok->head, expanded);
		}
		tok = tok->next;
	}
	return (0);
}

int	expand_redir_tildes(t_tokens *tokens)
{
	t_token	*tok;
	char	*expanded;

	if (tokens == NULL)
		return (0);
	tok = tokens->head;
	while (tok)
	{
		if (tok->has_tilde)
		{
			expanded = tilde_expand_word(tok->head->str);
			if (expanded)
				repl_and_free_str(tok->head, expanded);
		}
		tok = tok->next;
	}
	return (0);
}
