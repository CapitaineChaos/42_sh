/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_lifecycle.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 22:57:48 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 21:35:18 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_checker.h"
#include "module_context.h"
#include "utils.h"
#include "ft_std.h"
#include <stdbool.h>

void	checker_init(t_checker *checker, int lv)
{
	if (lv < 2)
		return ;
	trace_info(LVL_CHK, "Checker init");
	ft_memset(checker, 0, sizeof(t_checker));
	checker->proceed_loop = true;
}


void	checker_reset(t_checker *checker, int lv)
{
	if (lv < 2)
		return ;
	checker->parentheses = 0;
	checker->inp.stream = NULL;
	checker->inp.pos = 0;
	checker->current = NULL;
	checker->can_cmd = false;
	checker->cmd_ready = false;
	checker->pending_escape = false;
	checker->proceed_loop = true;
}
