/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_tok_type.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 21:47:28 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 01:04:48 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"

static char	*tok_to_str_words(t_tk_type t)
{
	if (TOK_UQUOTE == t)
		return ("UNQUOTED");
	if (TOK_DQUOTE == t)
		return ("DQUOTE");
	if (TOK_SQUOTE == t)
		return ("SQUOTE");
	if (TOK_GLOB == t)
		return ("GLOB");
	return (NULL);
}

static char	*tok_to_str_operators(t_tk_type t)
{
	if (TOK_PIPE == t)
		return ("PIPE");
	if (TOK_AND_IF == t)
		return ("AND_IF");
	if (TOK_OR_IF == t)
		return ("OR_IF");
	if (TOK_LPAREN == t)
		return ("LPAREN");
	if (TOK_RPAREN == t)
		return ("RPAREN");
	if (TOK_SEMI == t)
		return ("SEMI");
	return (NULL);
}

static char	*tok_to_str_redirections(t_tk_type t)
{
	if (TOK_REDIR_IN == t)
		return ("REDIR_IN");
	if (TOK_REDIR_OUT == t)
		return ("REDIR_OUT");
	if (TOK_REDIR_APPEND == t)
		return ("REDIR_APPEND");
	if (TOK_REDIR_HEREDOC == t)
		return ("REDIR_HEREDOC");
	return (NULL);
}

static char	*tok_to_str_other(t_tk_type t)
{
	if (TOK_EOF == t)
		return ("EOF");
	if (TOK_NEWLINE == t)
		return ("NEWLINE");
	if (TOK_WORD == t)
		return ("WORD");
	if (TOK_DELIM == t)
		return ("DELIM");
	if (TOK_OPERATOR == t)
		return ("OPERATOR");
	if (TOK_HEREDOC_BODY == t)
		return ("HEREDOC_BODY");
	return (NULL);
}

char	*debug_get_token_type(t_tk_type t)
{
	char	*str;

	str = tok_to_str_words(t);
	if (str)
		return (str);
	str = tok_to_str_operators(t);
	if (str)
		return (str);
	str = tok_to_str_redirections(t);
	if (str)
		return (str);
	str = tok_to_str_other(t);
	if (str)
		return (str);
	return ("      UNKNOWN");
}

char	*debug_get_tok_kind(t_tk_kind kind)
{
	if (kind == TKD_OPERAND)
		return ("OPERAND");
	if (kind == TKD_OPERATOR)
		return ("OPERATOR");
	if (kind == TKD_STRUCT)
		return ("STRUCT");
	else
		return ("UNKNOWN");
}
