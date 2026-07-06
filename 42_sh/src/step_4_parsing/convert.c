/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 02:23:12 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 00:05:42 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_parser.h"

/**
 * @brief  Récupère la priorité d'un type d'AST.
 * @param  type Le type d'AST.
 * @return La priorité du type d'AST.
 * @note   La priorité est utilisée pour déterminer l'ordre d'évaluation
 *         des opérations dans l'AST.
 */
size_t	get_precedence(t_ast_type type)
{
	if (type == AST_PIPE)
		return (3);
	else if (type == AST_OR || type == AST_AND)
		return (2);
	else if (type == AST_SEQ)
		return (1);
	else
		return (0);
}

/**
 * @brief  Convertit un type de token en type d'AST.
 * @param  type Le type de token à convertir.
 * @return Le type d'AST correspondant.
 * @note   Si le type de token n'est pas reconnu, retourne AST_UNKNOWN.
 */
t_ast_type	get_ast_type(t_tk_type type)
{
	if (type == TOK_PIPE)
		return (AST_PIPE);
	else if (type == TOK_AND_IF)
		return (AST_AND);
	else if (type == TOK_OR_IF)
		return (AST_OR);
	else if (type == TOK_SEMI)
		return (AST_SEQ);
	else if (type == TOK_NEWLINE)
		return (AST_SEQ);
	else if (type == TOK_LPAREN)
		return (AST_SUBSH_MARK);
	else if (type == TOK_RPAREN)
		return (AST_SUBSH_END);
	else
		return (AST_UNKNOWN);
}