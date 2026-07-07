/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_shortcuts.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 13:56:13 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 21:34:00 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_context.h"
#include "module_lexer.h"
#include <unistd.h>

int	print_syntax_error(char *s, char *line)
{
	t_logger	lg;

	log_init(&lg);
	log_puts(&lg, "🐰: syntax error near unexpected token `");
	log_puts(&lg, s);
	log_puts(&lg, "'\n");
	if (line && !isatty(STDIN_FILENO))
	{
		log_puts(&lg, "🐰: line: `");
		log_puts(&lg, line);
		if (lg.buf[lg.pos - 1] == '\n')
			lg.pos--;
		log_puts(&lg, "'\n");
	}
	log_flush(STDERR_FILENO, &lg, false);
	return (-1);
}

void	rem_op_context(t_lexer *lx)
{
	if (lx->ctxs.count > 0)
	{
		if (ctx_is_now(&lx->ctxs, CTX___CMDAND))
			remove_matching_context(&lx->ctxs, CTX___CMDAND);
		else if (ctx_is_now(&lx->ctxs, CTX___CMDOR))
			remove_matching_context(&lx->ctxs, CTX___CMDOR);
		else if (ctx_is_now(&lx->ctxs, CTX___PIPE))
			remove_matching_context(&lx->ctxs, CTX___PIPE);
	}
}

void	rem_sub_context(t_lexer *lx)
{
	if (lx->ctxs.count > 0)
	{
		if (ctx_is_now(&lx->ctxs, CTX___SUBSH))
			remove_matching_context(&lx->ctxs, CTX___SUBSH);
	}
}

void	rem_esc_context(t_lexer *lx)
{
	if (lx->ctxs.count > 0)
	{
		if (ctx_is_now(&lx->ctxs, CTX___ESCAPE))
			remove_matching_context(&lx->ctxs, CTX___ESCAPE);
	}
}

void	add_context(t_lexer *lx, t_tk_type type)
{
	if (type == TOK_LPAREN)
		context_add_(&lx->ctxs, CTX___SUBSH);
	else if (type == TOK_PIPE)
		context_add_(&lx->ctxs, CTX___PIPE);
	else if (type == TOK_AND_IF)
		context_add_(&lx->ctxs, CTX___CMDAND);
	else if (type == TOK_OR_IF)
		context_add_(&lx->ctxs, CTX___CMDOR); 
	else if (type == TOK_ESCAPE)
		context_add_(&lx->ctxs, CTX___ESCAPE);
}
