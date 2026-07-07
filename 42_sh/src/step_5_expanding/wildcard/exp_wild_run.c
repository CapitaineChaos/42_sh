/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_wild_run.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 00:46:07 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 20:19:41 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_token.h"
#include "module_expand.h"

static void	repl_token_with_globs(t_tokens *tks, t_token **rem, t_token *tklist)
{
	t_token	*old;
	t_token	*last_new;

	old = *rem;
	last_new = tklist;
	if (!tklist)
		return ;
	while (last_new->next)
		last_new = last_new->next;
	if (old->prev)
		old->prev->next = tklist;
	else
		tks->head = tklist;
	tklist->prev = old->prev;
	if (old->next)
		old->next->prev = last_new;
	else
		tks->tail = last_new;
	last_new->next = old->next;
	free_token(old);
	*rem = last_new;
}

t_token	*get_globs(t_tokens *toks, int *n, t_token **cur)
{
	t_token		*tmp;

	tmp = NULL;
	if (cur == NULL || *cur == NULL)
		return (NULL);
	if ((*cur)->has_glob && !(*cur)->has_quoted_part)
	{
		tmp = expand_wildcards(n, (*cur)->str);
	}
	if (*n == 0)
		return (NULL);
	toks->count += *n - 1;
	repl_token_with_globs(toks, cur, tmp);
	return (tmp);
}

/**
 * @brief Fonction pour étendre les jokers dans le répertoire courant.
 * @param pattern Le motif à étendre.
 * @param count Le nombre de correspondances trouvées.
 * @return t_token* Une liste de tokens correspondant aux fichiers trouvés.
 */
t_token	*expand_wildcards(int *count, const char *pattern)
{
	char		*dir_path;
	const char	*file_pattern;
	DIR			*dir;
	t_token		*result;

	result = NULL;
	dir_path = get_dir_path(pattern);
	file_pattern = get_file_pattern(pattern);
	dir = opendir(dir_path);
	if (!dir)
	{
		free(dir_path);
		return (NULL);
	}
	*count += add_matches_from_dir(&result, dir, dir_path, file_pattern);
	closedir(dir);
	free(dir_path);
	return (result);
}

bool	expand_cmd_wildcards(t_tokens *tokens)
{
	t_token	*token;
	int		n;

	if (tokens == NULL)
		return (true);
	token = tokens->head;
	while (token)
	{
		n = 0;
		if (token->has_glob && !token->has_quoted_part)
			get_globs(tokens, &n, &token);
		token = token->next;
	}
	return (true);
}

int	expand_redir_wildcards(t_tokens *tokens)
{
	t_token	*token;
	int		n;

	if (tokens == NULL)
		return (0);
	token = tokens->head;
	while (token)
	{
		if (token->family == TKF_WORD && token->type != TOK_DELIM)
		{
			n = 0;
			if (token->has_glob && !token->has_quoted_part)
				get_globs(tokens, &n, &token);
			if (n != 1)
				return (n);
		}
		token = token->next;
	}
	return (0);
}
