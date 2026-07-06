/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 00:43:25 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 19:21:53 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_lexer.h"
#include "module_token.h"
#include "module_expand.h"
#include "module_normalize.h"
#include "module_read.h"
#include <readline/history.h>

char	*collect_body(t_lexer *lx, t_token *delim, int i, int line_count)
{
	char		*content;
	char		*str_delim;
	char		*prompt;

	assign_parts_ptrs(delim, lx->inp.stream);
	assign_string(delim);
	normalize_token(delim);
	str_delim = aggregate_final_token_parts(delim->head);
	prompt = get_heredoc_prompt(i, lx->heredoc_count);
	sstrs_append(&lx->garbage, str_delim);
	sstrs_append(&lx->garbage, prompt);
	if (read_heredoc_body(prompt, str_delim, &lx->heredoc_garbage, line_count))
	{
		content = sstrs_flatten(lx->heredoc_garbage);
		sstrs_append(&lx->heredoc_garbage, content);
		return (content);
	}
	return (NULL);
}

bool	collect_heredocs(t_lexer *lx, int line_count, int lv)
{
	t_token		*token;
	char		*content;
	int			i;

	if (lv < 3 || lx->heredoc_count == 0)
		return (true);
	token = lx->tokens.head;
	i = 1;
	while (token)
	{
		while (token && token->type != TOK_REDIR_HEREDOC)
			token = token->next;
		if (token && token->tk_flag == true)
		{
			token->tk_flag = false;
			content = collect_body(lx, token->next, i, line_count);
			if (content == NULL)
				return (false);
			tk_heredoc_body_emit(lx, token->next, content);
		}
		i++;
		if (token)
			token = token->next;
	}
	return (true);
}
