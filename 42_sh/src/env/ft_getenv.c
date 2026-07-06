/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 02:09:54 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/05 01:45:03 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_std.h"
#include "module_env.h"
#include <unistd.h>

/**
 * @brief		Renvoie la valeur d'une variable d'environnement.
 * @param env	Liste chaînée des variables d'environnement.
 * @param name	Nom de la variable d'environnement.
 * @return		Valeur de la variable d'environnement ou NULL si n'existe pas.
 * @note		Pas de dup/malloc sur la valeur de retour.
 */
char	*ft_getenv(t_env_lst *env, char *name)
{
	t_env_node	*cur;

	cur = env->head;
	if (!name || !env)
		return (NULL);
	while (cur)
	{
		if (ft_strcmp(cur->key, name) == 0)
			return (cur->value);
		cur = cur->next;
	}
	return (NULL);
}
