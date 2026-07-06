/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_tok_tclass.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:45:44 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/25 02:27:53 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"

char	*get_tok_family(t_tk_family family)
{
	if (family == TKF_WORD)
		return ("         WORD");
	if (family == TKF_OPERATOR)
		return ("     OPERATOR");
	if (family == TKF_REDIRECT)
		return ("     REDIRECT");
	if (family == TKF_HEREDOC_BODY)
		return (" HEREDOC_BODY");
	if (family == TKF_SUBSHELL)
		return ("     SUBSHELL");
	if (family == TKF_WORD_REDIR)
		return ("   WORD_REDIR");
	return ("      UNKNOWN");
}
