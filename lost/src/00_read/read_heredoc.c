/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 13:19:37 by smaitre           #+#    #+#             */
/*   Updated: 2025/05/28 20:37:17 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "ft_std.h"
#include "module_signal.h"
#include <readline/readline.h>
#include <stdlib.h>

char	*get_heredoc_prompt(int i, int count)
{
	t_logger	lg;

	log_init(&lg);
	log_puts(&lg, "heredoc(");
	log_putlong(&lg, i);
	log_puts(&lg, "/");
	log_putlong(&lg, count);
	log_puts(&lg, ")> ");
	log_putch(&lg, '\0');
	return (strdup(lg.buf));
}

void	print_cancel(int line, char *delim)
{
	t_logger	lg;

	log_init(&lg);
	log_puts(&lg, "🐯: warning: here-document at line ");
	log_putlong(&lg, line);
	log_puts(&lg, " delimited by end-of-file (wanted `");
	log_puts(&lg, delim);
	log_puts(&lg, "')\n");
	log_flush(STDERR_FILENO, &lg, false);
}

static char	*get_line(char *prompt)
{
	char	*line;
	size_t	len;

	if (isatty(STDIN_FILENO))
	{
		signal(SIGINT, sigint_heredoc);
		line = readline(prompt);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		line = nano_get_next_line(STDIN_FILENO);
		if (line && line[0])
		{
			len = strlen(line);
			if (len && line[len - 1] == '\n')
				line[len - 1] = '\0';
		}
	}
	return (line);
}

bool	read_heredoc_body(char *prompt, char *delim, t_sstr **body, int lc)
{
	char		*content;

	sstr_free(body);
	while (true)
	{
		signal(SIGINT, sigint_heredoc);
		content = get_line(prompt);
		if (!isatty(STDIN_FILENO) && content == NULL)
		{
			if (sstrs_count(*body) > 0)
				return (print_cancel(lc, delim), true);
		}
		if (content == NULL)
			return (print_cancel(lc, delim), false);
		if (strcmp(content, delim) == 0 || g_signal_flag == 1)
		{
			free(content);
			return (g_signal_flag == 0);
		}
		sstrs_append(body, content);
		sstrs_append(body, strdup("\n"));
	}
	return (true);
}
