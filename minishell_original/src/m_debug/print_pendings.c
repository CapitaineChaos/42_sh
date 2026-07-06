/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_pendings.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 01:15:21 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/27 16:31:45 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_checker.h"
#include <stdio.h>


void	debug_chk_pendings(t_checker *chk, char *msg)
{
	char		buff[1024];

	if (DLVL < LVL_CHK)
		return ;
	sprintf(buff, "[%s] > Pendings: CMD[%c] RDR[%c] ESC[%c], %s\n",
		convert_dbg_lvl_name(LVL_CHK),
		chk->cmd_ready ? '+' : '-',
		chk->can_cmd ? '+' : '-',
		chk->pending_escape ? '+' : '-',
		msg);
	debug_log(buff);
}

void	debug_lx_pendings(t_lexer *lx)
{
	char		buff[1024];

	if (DLVL < LVL_LEXER)
		return ;
	sprintf(buff, "[%s] > Pendings: DQ[%c] SQ[%c] ESC[%c] HD[%d]\n",
		convert_dbg_lvl_name(LVL_LEXER),
		lx->pending_dquote ? '+' : '-',
		lx->pending_squote ? '+' : '-',
		lx->pending_escape ? '+' : '-',
		lx->heredoc_count);
	debug_log(buff);
}

void	debug_contexts(t_contexts *ctxs)
{
	char		buff[1024];
	t_context	*ctx_node;

	if (DLVL < LVL_CHK)
		return ;
	sprintf(buff, "[%s] > Contexts [%ld]: ", convert_dbg_lvl_name(LVL_CHK), ctxs->count);
	debug_log(buff);
	ctx_node = ctxs->head;
	while (ctx_node)
	{
		sprintf(buff, "%s ", convert_ctx_to_sym(ctx_node->ctx_name));
		debug_log(buff);
		ctx_node = ctx_node->next;
	}
	sprintf(buff, "\n");
	debug_log(buff);

}
