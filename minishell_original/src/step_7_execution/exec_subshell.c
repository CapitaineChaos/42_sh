/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 22:35:44 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 20:10:07 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_exec.h"
#include "module_parser.h"
#include <sys/wait.h>

/**
 * @brief Exécute un sous-shell.
 * @note Fork un processus fils pour exécuter le nœud gauche.
 * @param node Le nœud de l'arbre AST à exécuter.
 * @return Le code de retour du processus fils, ou EXIT_FAILURE en cas d'erreur.
 */
int	exec_subshell(t_ast_node *node)
{
	pid_t	pid;
	int		status;
	int		sv_in;
	int		sv_out;

	pid = fork();
	if (pid < 0)
		return (EXIT_FAILURE);
	if (pid == 0)
	{
		sv_in = dup(STDIN_FILENO);
		sv_out = dup(STDOUT_FILENO);
		if (sv_in < 0 || sv_out < 0)
			free_and_exit_minishell(EXIT_FAILURE);
		if (apply_redirs(node) != 0)
			free_and_exit_minishell(EXIT_FAILURE);
		status = exec_node(node->left);
		free_and_exit_minishell(get_exit_status(status));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE);
}
