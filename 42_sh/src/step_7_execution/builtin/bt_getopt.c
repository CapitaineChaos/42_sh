/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bt_getopt.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 00:00:00 by smaitre           #+#    #+#             */
/*   Updated: 2026/07/07 00:00:00 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_builtin.h"
#include <string.h>

static int	opt_index(const char *valid, char c)
{
	int	i;

	i = 0;
	while (valid && valid[i])
	{
		if (valid[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

static int	long_index(const char *const *longs, const char *name)
{
	int	i;

	i = 0;
	while (longs && longs[i])
	{
		if (!strcmp(longs[i], name))
			return (i);
		i++;
	}
	return (-1);
}

static void	res_init(t_optres *r)
{
	r->flags = 0;
	r->lflags = 0;
	r->last = 0;
	r->count = 0;
	r->operand = 0;
	r->bad = 0;
	r->badarg = NULL;
}

static int	parse_short(char *arg, const char *shorts, t_optres *r)
{
	int	j;
	int	k;

	j = 0;
	while (arg[++j])
	{
		k = opt_index(shorts, arg[j]);
		if (k < 0)
			return (r->bad = arg[j], r->badarg = arg, -1);
		r->flags |= (1u << k);
		r->last = arg[j];
		r->count++;
	}
	return (0);
}

/*
 * Parse les options en tête de argv[1..]. Option courte = arg "-xyz" (chaque
 * char dans `shorts`, combinable), option longue = "--name" (name dans
 * `longs`). "--", "-" seul, ou le 1er non-option arrêtent le parsing.
 * `shorts`/`longs` peuvent être NULL. Retourne 0 si tout est valide, -1 sinon.
 */
int	bt_getopt(char **argv, const char *shorts,
		const char *const *longs, t_optres *r)
{
	int	i;
	int	k;

	res_init(r);
	i = 0;
	while (argv[++i])
	{
		if (!strcmp(argv[i], "--"))
			return (r->operand = i + 1, 0);
		if (argv[i][0] != '-' || argv[i][1] == '\0')
			return (r->operand = i, 0);
		if (argv[i][1] == '-')
		{
			k = long_index(longs, argv[i] + 2);
			if (k < 0)
				return (r->badarg = argv[i], -1);
			r->lflags |= (1u << k);
		}
		else if (parse_short(argv[i], shorts, r) < 0)
			return (-1);
	}
	r->operand = i;
	return (0);
}

bool	bt_optset(const t_optres *r, const char *shorts, char c)
{
	int	k;

	k = opt_index(shorts, c);
	if (k < 0)
		return (false);
	return ((r->flags & (1u << k)) != 0);
}

bool	bt_optlong(const t_optres *r, const char *const *longs, const char *name)
{
	int	k;

	k = long_index(longs, name);
	if (k < 0)
		return (false);
	return ((r->lflags & (1u << k)) != 0);
}
