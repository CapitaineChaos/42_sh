/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:18:51 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 00:22:03 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_token.h"
#include "module_lexer.h"
#include "module_env.h"
#include "ft_strapi.h"
#include "ft_std.h"

/* Initialiser le lexer une seule fois*/
void	lexer_init(t_lexer *lx, char *ifs, int lv)
{
	if (lv < 3)
		return ;
	trace_start(LVL_LEXER, "Init");
	memset(lx, 0, sizeof (t_lexer));
	lx->ifs = ifs;
	trace_ok(LVL_LEXER, "Init");
}

/* Libérer définitivement le lexer */
void	lexer_free(t_lexer *lx, int lv)
{
	if (lv < 3)
		return ;
	free_token_parts(lx->tmp_wp);
	contexts_free(&lx->ctxs);
	trace_info(LVL_LEXER, "Temp token parts freed");
	lx->tmp_wp = NULL;
	free_token_list(&lx->tokens);
	sstr_free(&lx->heredoc_garbage);
	sstr_free(&lx->garbage);
	*lx = (t_lexer){0};
	lx->inp.stream = NULL;
	lx->inp.pos = 0;
	trace_info(LVL_LEXER, "Lexer freed");
}

void	lexer_hard_reset(t_lexer *lx, int lv)
{
	if (lv < 3)
		return ;
	free_token_parts(lx->tmp_wp);
	trace_info(LVL_LEXER, "Temp token parts freed");
	lx->tmp_wp = NULL;
	free_token_list(&lx->tokens);
	contexts_free(&lx->ctxs);
	sstr_free(&lx->heredoc_garbage);
	sstr_free(&lx->garbage);
	lx->pending_escape = false;
	lx->pending_squote = false;
	lx->pending_dquote = false;
	lx->heredoc_count = 0;
	lx->inp.stream = NULL;
	lx->inp.pos = 0;
	lx->tokens.head = NULL;
	lx->tokens.tail = NULL;
	lx->tokens.count = 0;
	trace_ok(LVL_LEXER, "Hard reset ended");
}

/* Réinitialiser le lexer pour une nouvelle utilisation */
void	lexer_soft_reset(t_lexer *lx, int lv)
{
	if (lv < 3)
		return ;
	free_token_parts(lx->tmp_wp);
	contexts_free(&lx->ctxs);
	trace_info(LVL_LEXER, "Temp token parts freed");
	lx->tmp_wp = NULL;
	lx->pending_escape = false;
	lx->pending_squote = false;
	lx->pending_dquote = false;
	lx->heredoc_count = 0;
	lx->inp.stream = NULL;
	lx->inp.pos = 0;
	trace_start(LVL_LEXER, "Soft reset ended");
}
