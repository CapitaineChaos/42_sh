/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx_lifecycle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 18:00:22 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/23 03:10:15 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_context.h"
#include <stdlib.h>

void	contexts_init(t_contexts *ctxs)
{
	ctxs->head = NULL;
	ctxs->tail = NULL;
	ctxs->count = 0;
}

void	contexts_free(t_contexts *ctxs)
{
	t_context	*ctx;

	if (ctxs->count == 0 || ctxs->head == NULL)
	{
		return ;
	}
	ctx = ctxs->head;
	while (ctx)
	{
		ctxs->head = ctx->next;
		free(ctx);
		ctxs->count--;
		ctx = ctxs->head;
	}
	ctxs->tail = NULL;
	ctxs->head = NULL;
	ctxs->count = 0;
}
