/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_context.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 23:41:38 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 01:02:22 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODULE_CONTEXT_H
# define MODULE_CONTEXT_H

# include <stdbool.h>
# include <stddef.h>

typedef enum e_ctx_
{
	CTX___ESCAPE,
	CTX___SQUOTE,
	CTX___DQUOTE,
	CTX___SUBSH,
	CTX___CMDAND,
	CTX___CMDOR,
	CTX___PIPE,
	CTX___NONE,
}	t_ctx_;

typedef struct s_context
{
	t_ctx_				ctx_name;
	struct s_context	*next;
	struct s_context	*prev;
}	t_context;

typedef struct s_contexts
{
	t_context	*head;
	t_context	*tail;
	size_t		count;
}	t_contexts;

bool	is_hidden_context(t_ctx_ ctx_name);
bool	ctx_is_now(t_contexts *ctxs, t_ctx_ ctx_name);
void	context_add_(t_contexts *ctxs, t_ctx_ ctx_name);
bool	context_remove_last_(t_contexts *ctxs, t_ctx_ ctx_name);
bool	remove_matching_context(t_contexts *ctxs, t_ctx_ ctx_name);

void	contexts_init(t_contexts *ctxs);
void	contexts_free(t_contexts *ctxs);

char	*convert_ctx(t_ctx_ s);
char	*convert_contexts_to_str_(t_contexts *ctxs);
char	*convert_ctx_to_sym(t_ctx_ s);

#endif
