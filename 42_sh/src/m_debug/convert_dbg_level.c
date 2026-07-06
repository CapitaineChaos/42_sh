/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_dbg_level.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 01:56:57 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/14 01:13:51 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_config.h"

char	*convert_dbg_lvl_name_suite(int lvl)
{
	if (lvl == LVL_EXEC)
		return ("⚙️ EXC");
	else if (lvl == LVL_OK)
		return ("📨 MSG");
	else if (lvl == LVL_FAIL)
		return ("❌ ERR");
	else if (lvl == LVL_CHK)
		return ("🔎 CHK");
	else if (lvl == LVL_INPUT)
		return ("💬 INP");
	else if (lvl == LVL_MNS)
		return ("🐰 MNS");
	else
		return ("🔥 UNK");
}

char	*convert_dbg_lvl_name(int lvl)
{
	if (lvl == LVL_LEXER)
		return ("🔮 LXR");
	else if (lvl == LVL_HEREDOC)
		return ("📥 HDC");
	else if (lvl == LVL_PARSER)
		return ("🧩 PRS");
	else if (lvl == LVL_AST)
		return ("🌳 AST");
	else if (lvl == LVL_EXPAND)
		return ("🔰 EXP");
	else if (lvl == LVL_ARGV)
		return ("📋 ARG");
	else
		return (convert_dbg_lvl_name_suite(lvl));
}
