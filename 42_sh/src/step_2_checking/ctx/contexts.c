/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contexts.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:21:58 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 01:16:46 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_strapi.h"
#include "module_context.h"
#include "module_debug.h"
#include "module_minishell.h"
#include <stdlib.h>

static char	*ctx_names[] = {
	"concat",
	"squote",
	"dquote",
	"subshell",
	"cmdand",
	"cmdor",
	"pipe",
	"none",
};

static char	*ctx_symbols[] = {
	"\\",
	"'",
	"\"",
	"(",
	"&&",
	"||",
	"|",
	"none",
};

char	*convert_ctx(t_ctx_ ctx)
{
	if (ctx < CTX___ESCAPE || ctx > CTX___NONE)
		return ("UNKNOWN");
	return (ctx_names[ctx]);
}

char	*convert_ctx_to_sym(t_ctx_ ctx)
{
	if (ctx < CTX___ESCAPE || ctx > CTX___NONE)
		return ("UNKNOWN");
	return (ctx_symbols[ctx]);
}

bool	is_hidden_context(t_ctx_ ctx_name)
{
	return (ctx_name == CTX___NONE);
}

bool	ctx_is_now(t_contexts *ctxs, t_ctx_ ctx_name)
{
	return (ctxs->tail && ctxs->tail->ctx_name == ctx_name);
}

void	contexts_init(t_contexts *ctxs)
{
	trace_start(LVL_CHK, "Checker contexts init");
	ctxs->head = NULL;
	ctxs->tail = NULL;
	ctxs->count = 0;
}

void	contexts_free(t_contexts *ctxs)
{
	t_context	*ctx;

	trace_start(LVL_CHK, "Checker contexts free");
	if (ctxs->count == 0 || ctxs->head == NULL)
	{
		trace_info(LVL_CHK, "Checker contexts free : None");
		return ;
	}
	trace_info_nvnb(LVL_CHK, "Checker contexts to free : ", ctxs->count);
	while (ctxs->head)
	{
		ctx = ctxs->head;
		ctxs->head = ctx->next;
		free(ctx);
		ctxs->count--;
	}
	ctxs->tail = NULL;
	if (ctxs->count != 0)
		trace_info(LVL_FAIL, "  @@@ Memory leak in checker ctx  ");
	ctxs->count = 0;
}

void	context_add_(t_contexts *ctxs, t_ctx_ ctx_name)
{
	t_context	*ctx_node;

	if (is_hidden_context(ctx_name))
	{
		trace_info(LVL_CHK, "Context is hidden");
		return ;
	}
	ctx_node = calloc(1, sizeof(t_context));
	if (!ctx_node)
	{
		trace_info(LVL_FAIL, "Computer memory failure");
		free_and_exit_minishell(EXIT_FAILURE);
	}
	ctx_node->ctx_name = ctx_name;
	ctx_node->prev = ctxs->tail;
	if (ctxs->tail)
		ctxs->tail->next = ctx_node;
	ctxs->tail = ctx_node;
	if (!ctxs->head)
		ctxs->head = ctx_node;
	ctxs->count++;
}

bool	remove_matching_context(t_contexts *ctxs, t_ctx_ ctx_name)
{
	t_context	*ctx_node;

	ctx_node = ctxs->tail;
	while (ctx_node && ctx_node->ctx_name != ctx_name)
		ctx_node = ctx_node->prev;
	if (!ctx_node)
		return (false);
	if (ctx_node->prev)
		ctx_node->prev->next = ctx_node->next;
	else
		ctxs->head = ctx_node->next;
	if (ctx_node->next)
		ctx_node->next->prev = ctx_node->prev;
	else
		ctxs->tail = ctx_node->prev;
	free(ctx_node);
	ctxs->count--;
	return (true);
}

bool	context_remove_last_(t_contexts *ctxs, t_ctx_ ctx_name)
{
	t_context	*ctx_node;

	ctx_node = ctxs->tail;
	while (ctx_node && is_hidden_context(ctx_node->ctx_name))
		ctx_node = ctx_node->prev;
	if (!ctx_node)
		return (false);
	if (ctx_node->ctx_name != ctx_name)
	{
		trace_info(LVL_FAIL, "@@@ Context not matching");
		return (false);
	}
	ctxs->tail = ctx_node->prev;
	if (ctxs->tail)
		ctxs->tail->next = NULL;
	else
		ctxs->head = NULL;
	free(ctx_node);
	ctxs->count--;
	return (true);
}

char	*convert_contexts_to_str_(t_contexts *ctxs)
{
	t_strapi	s;
	t_context	*ctx_node;
	char		*str;

	strapi_init(4096, &s);
	ctx_node = ctxs->head;
	while (ctx_node)
	{
		if (!is_hidden_context(ctx_node->ctx_name))
		{
			str = convert_ctx(ctx_node->ctx_name);
			if (ctx_node->prev != NULL)
				s.append_char(&s, ' ');
			s.append_str(&s, str);
		}
		ctx_node = ctx_node->next;
	}
	s.append_str(&s, "> ");
	str = s.get_dup(&s);
	s.free(&s);
	return (str);
}
