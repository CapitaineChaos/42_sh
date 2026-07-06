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

#include "module_debug.h"
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
	trace_logger_flush(-1, &lg, true);
}

/**
 * @brief Fonction principale de la phase de parsing.
 * @param mns La structure de données principale du minishell.
 * @return true si le parsing a réussi, false sinon.
 * @note Cette fonction vérifie si la liste de tokens est vide, puis
 * 	 prépare la structure de parsing et construit l'arbre AST.
 */
bool	run_parser(t_parser *prs, t_tokens *tkns, int lv)
{
	if (lv < 4)
		return (false);
	debug_title(LVL_PARSER, "[  Parsing  ]");
	if (tkns->count == 0)
	{
		trace_info(LVL_PARSER, "Aucun token à parser");
		return (false);
	}
	tokens_to_nodes(tkns, &prs->deques.input);
	debug_deque(&prs->deques.input, "INPUT");
	apply_shunting_yard(&prs->deques);
	debug_deque(&prs->deques.input, "IN (empty) Shunting yard");
	debug_deque(&prs->deques.operators, "OPS (empty) Shunting yard");
	debug_deque(&prs->deques.output, "RPN OUT Shunting yard");
	build_ast(&prs->deques.final, &prs->deques.output);
	debug_ast_tree(&prs->deques.final);
	dbg_ast(&prs->deques.final);
	debug_check_integrity(prs);
	if (prs->deques.final.size == 0)
		return (false);
	return (true);
}
