/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx_convert.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 22:23:27 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/30 01:16:46 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_context.h"

char	*convert_ctx_to_str_suite(t_ctx_ s)
{
	if (s == CTX___ESCAPE)
		return ("concat");
	else if (s == CTX___CMDAND)
		return ("cmdand");
	else if (s == CTX___CMDOR)
		return ("cmdor");
	else if (s == CTX___PIPE)
		return ("pipe");
	else if (s == CTX___NONE)
		return ("none");
	else
		return ("UNKNOWN");
}

char	*convert_ctx(t_ctx_ s)
{
	if (s == CTX___SUBSH)
		return ("subshell");
	else if (s == CTX___DQUOTE)
		return ("dquote");
	else if (s == CTX___SQUOTE)
		return ("squote");
	else
		return (convert_ctx_to_str_suite(s));
}

char	*convert_ctx_to_sym_suite(t_ctx_ s)
{
	if (s == CTX___ESCAPE)
		return ("\\");
	else if (s == CTX___CMDAND)
		return ("&&");
	else if (s == CTX___CMDOR)
		return ("||");
	else if (s == CTX___PIPE)
		return ("|");
	else if (s == CTX___NONE)
		return ("none");
	else
		return ("UNKNOWN");
}

char	*convert_ctx_to_sym(t_ctx_ s)
{
	if (s == CTX___SUBSH)
		return ("(");
	else if (s == CTX___DQUOTE)
		return ("\"");
	else if (s == CTX___SQUOTE)
		return ("'");
	else
		return (convert_ctx_to_sym_suite(s));
}
