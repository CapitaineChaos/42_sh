/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_deque.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 17:09:43 by smaitre           #+#    #+#             */
/*   Updated: 2025/04/29 19:27:51 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DEQUE_H
# define FT_DEQUE_H

# include <stddef.h>

typedef struct s_dq_n	t_dq_n;
typedef struct s_deque	t_deque;

typedef struct s_dq_n
{
	void			*node;
	struct s_dq_n	*next;
	struct s_dq_n	*prev;
}	t_dq_n;

typedef struct s_deque
{
	t_dq_n	*head;
	t_dq_n	*tail;
	size_t	size;
	char	*name;
}	t_deque;

t_dq_n	*deque_pop_front(t_deque *deque);
void	deque_push_front(t_deque *deque, t_dq_n *node);
t_dq_n	*deque_pop_back(t_deque *deque);
void	deque_push_back(t_deque *deque, t_dq_n *node);

t_dq_n	*dstn_new(void *foreign_node);
void	deque_node_free(t_dq_n *node, void (*del)(void *));
void	deque_free(t_deque *deque, void (*del)(void *));
void	deque_init(t_deque *deque);
void	dstk_reset(t_deque *deque);

#endif
