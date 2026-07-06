/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 00:31:35 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 21:10:09 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_token.h"
#include "module_expand.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

static void	advance_ptrs(char **a, char **b)
{
	while (**a && !isalnum(**a))
		(*a)++;
	while (**b && !isalnum(**b))
		(*b)++;
}

static int	glob_cmp(char *new_str, char *existing)
{
	unsigned char		ca;
	unsigned char		cb;
	unsigned char		la;
	unsigned char		lb;

	while (*new_str || *existing)
	{
		advance_ptrs(&new_str, &existing);
		ca = *new_str;
		cb = *existing;
		if (!ca || !cb)
			break ;
		la = tolower((ca));
		lb = tolower(cb);
		if (la != lb)
			return (la - lb);
		if ((ca) != cb)
			return ((ca) - cb);
		new_str++;
		existing++;
	}
	return ((int)ca - (int)cb);
}

void	insert_sorted(t_token **head, t_token *new)
{
	t_token	*cur;
	t_token	*prev;

	if (!*head || glob_cmp(new->str, (*head)->str) < 0)
	{
		new->next = *head;
		*head = new;
		return ;
	}
	prev = *head;
	cur = (*head)->next;
	while (cur && glob_cmp(new->str, cur->str) > 0)
	{
		prev = cur;
		cur = cur->next;
	}
	new->next = cur;
	prev->next = new;
}
