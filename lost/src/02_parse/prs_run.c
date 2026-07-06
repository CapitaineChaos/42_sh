/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prs_run.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 23:41:54 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 20:10:23 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_token.h"
#include "module_parser.h"

void	print_wildcat_error(char *str)
{
	t_logger	lg;

	log_init(&lg);
	log_puts(&lg, "🐯: syntax error near unexpected token `");
	log_puts(&lg, str);
	log_puts(&lg, "'\n");
	log_flush(STDERR_FILENO, &lg, false);
}

bool	run_parser(t_parser *prs, t_tokens *tkns, int lv)
{
	if (lv < 4)
		return (false);
	if (tkns->count == 0)
	{
		return (false);
	}
	tokens_to_nodes(tkns, &prs->deques.input);
	apply_shunting_yard(&prs->deques);
	build_ast(&prs->deques.final, &prs->deques.output);
	if (prs->deques.final.size == 0)
		return (false);
	return (true);
}
