/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_apply.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:55:57 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/29 20:07:15 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_exec.h"
#include "module_minishell.h"
#include "module_builtin.h"
#include "module_parser.h"
#include "module_token.h"
#include "module_expand.h"
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

static int	dup_close(int oldfd, int newfd)
{
	int	saved_errno;

	if (oldfd == newfd)
		return (EXIT_SUCCESS);
	if (dup2(oldfd, newfd) < 0)
	{
		saved_errno = errno;
		close(oldfd);
		if (saved_errno != EPIPE)
			perror("dup2");
		errno = saved_errno;
		return (EXIT_FAILURE);
	}
	close(oldfd);
	return (EXIT_SUCCESS);
}

int	apply_redir_heredoc(char *content)
{
	int		fd[2];
	ssize_t	len;
	ssize_t	written;
	ssize_t	n;

	if (pipe(fd) < 0)
		return (perror("pipe"), EXIT_FAILURE);
	len = (ssize_t)strlen(content);
	written = 0;
	while (written < len)
	{
		n = write(fd[1], content + written, len - written);
		if (n < 0)
		{
			perror("write");
			close(fd[0]);
			close(fd[1]);
			return (EXIT_FAILURE);
		}
		written += n;
	}
	close(fd[1]);
	return (dup_close(fd[0], STDIN_FILENO));
}

typedef struct s_redir_spec
{
	t_tk_type	type;
	int			flags;
	int			mode;
	int			target;
}	t_redir_spec;

/* Correspondance redirection -> flags open + fd cible. */
static const t_redir_spec	g_redirs[] = {
	{TOK_REDIR_OUT, O_CREAT | O_WRONLY | O_TRUNC | O_CLOEXEC, 0666, STDOUT_FILENO},
	{TOK_REDIR_IN, O_RDONLY, 0, STDIN_FILENO},
	{TOK_REDIR_APPEND, O_CREAT | O_WRONLY | O_APPEND, 0644, STDOUT_FILENO},
};

int	apply_redir_other(t_token *tok, char *target)
{
	size_t	i;
	int		fd;

	for (i = 0; i < sizeof(g_redirs) / sizeof(g_redirs[0]); i++)
	{
		if (g_redirs[i].type != tok->type)
			continue ;
		fd = open(target, g_redirs[i].flags, g_redirs[i].mode);
		if (fd < 0)
			return (perror("open"), EXIT_FAILURE);
		return (dup_close(fd, g_redirs[i].target));
	}
	return (EXIT_FAILURE);
}

int	apply_redirs(t_ast_node *node)
{
	t_token	*tok;

	if (expand_redirs(node) == -1)
		return (EXIT_FAILURE);
	tok = node->t_ast_data.operand.redirections.head;
	while (tok)
	{
		if (tok->group == TKG_REDIRECTION)
		{
			if (tok->type == TOK_REDIR_HEREDOC && tok->next && tok->next->next
				&& tok->next->next->type == TOK_HEREDOC_BODY)
			{
				if (apply_redir_heredoc(tok->next->next->str) != 0)
					return (EXIT_FAILURE);
			}
			else if (tok->next && tok->next->type == TOK_WORD)
			{
				if (apply_redir_other(tok, tok->next->str) != 0)
					return (EXIT_FAILURE);
			}
		}
		tok = tok->next;
	}
	return (EXIT_SUCCESS);
}
