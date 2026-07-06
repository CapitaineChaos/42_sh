/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   module_normalize.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 18:55:04 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 19:21:54 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODULE_NORMALIZE_H
# define MODULE_NORMALIZE_H

# include "module_token.h"
# include "all_config.h"

char	*normalize_part(t_tk_part *part);
void	normalize_parts(t_tk_part *first);

void	normalize_tokens(t_tokens *tokens);
void	normalize_token(t_token *token);

void	assign_strings(t_tokens *tokens);
void	assign_string(t_token *token);
void	assign_parts_ptrs(t_token *token, char *stream);

#endif
