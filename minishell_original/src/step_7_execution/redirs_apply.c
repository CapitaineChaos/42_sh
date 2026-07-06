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

#include "module_debug.h"
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
	len = (ssize_t)ft_strlen(content);
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

int	apply_redir_other(t_token *tok, char *target)
{
	int	fd;

	if (tok->type == TOK_REDIR_OUT)
		fd = open(target, O_CREAT | O_WRONLY | O_TRUNC | O_CLOEXEC, 0666);
	else if (tok->type == TOK_REDIR_IN)
		fd = open(target, O_RDONLY);
	else if (tok->type == TOK_REDIR_APPEND)
		fd = open(target, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = -1;
	if (fd < 0)
		return (perror("open"), EXIT_FAILURE);
	if (tok->type == TOK_REDIR_IN)
		return (dup_close(fd, STDIN_FILENO));
	else
		return (dup_close(fd, STDOUT_FILENO));
}

int	apply_redirs(t_ast_node *node)
{
	t_token	*tok;

	if (expand_redirs(node) == -1)
		return (EXIT_FAILURE);
	debug_mini_title(LVL_EXEC, "Applying redirections");
	tok = node->t_ast_data.operand.redirections.head;
	while (tok)
	{
		if (tok->group == TKG_REDIRECTION)
		{
			trace_info_nvstr(LVL_EXEC, "(APPLY) Token type", debug_get_token_type(tok->type));			
			if (tok->type == TOK_REDIR_HEREDOC && tok->next && tok->next->next
				&& tok->next->next->type == TOK_HEREDOC_BODY)
			{
				trace_info_nvstr(LVL_EXEC, "(APPLY) Redir target", tok->next->next->str);
				if (apply_redir_heredoc(tok->next->next->str) != 0)
					return (EXIT_FAILURE);
			}
			else if (tok->next && tok->next->type == TOK_WORD)
			{
				trace_info_nvstr(LVL_EXEC, "(APPLY) Redir target", tok->next->str);
				if (apply_redir_other(tok, tok->next->str) != 0)
					return (EXIT_FAILURE);
			}
		}
		tok = tok->next;
	}
	return (EXIT_SUCCESS);
}
