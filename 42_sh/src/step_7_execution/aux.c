/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaitre <smaitre@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 19:39:49 by smaitre           #+#    #+#             */
/*   Updated: 2025/06/01 01:32:41 by smaitre          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "module_debug.h"
#include "module_exec.h"
#include "module_env.h"
#include "utils.h"
#include "ft_std.h"
#include <errno.h>
#include <sys/stat.h>

bool	is_directory(const char *path)
{
	struct stat	sb;

	if (stat(path, &sb) == 0)
		return (S_ISDIR(sb.st_mode));
	return (false);
}

int	write_buffer(int fd, t_logger *lg)
{
	int			ret;
	
	ret = write(fd, lg->buf, lg->pos);
	if (ret < 0)
	{
		if (errno == EPIPE)
		{
			log_init(lg);
			log_puts(lg, "🐰🐯 Minishell: ");
			log_puts(lg, "Broken pipe\n");
			log_flush(STDERR_FILENO, lg, false);
		}
		return (1);
	}
	return (0);
}

bool	has_slash(const char *s)
{
	while (*s)
		if (*s++ == '/')
			return (true);
	return (false);
}

static char	*get_good_path(char *exec, char *path)
{
	char	**paths;
	char	*full_path;
	int		i;
	char	*tmp;

	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(tmp, exec);
		if (tmp)
			free(tmp);
		if (access(full_path, X_OK) == 0)
		{
			free_char_array(paths);
			return (full_path);
		}
		free(full_path);
	}
	return (free_char_array(paths), NULL);
}

/**
 * @brief Get the executable path from the command.
 * 
 * If the command contains a slash, it is considered a direct path.
 * Otherwise, it searches for the command in the PATH environment variable.
 * 
 * @param mns The minishell structure containing environment variables.
 * @param argv The command arguments.
 * @return il faut FREE
 */
char	*get_exec_path(t_mns *mns, char **argv)
{
	char		*exec_path;

	(void)mns;
	if (has_slash(argv[0]))
		exec_path = strdup(argv[0]);
	else
		exec_path = get_good_path(argv[0], getenv("PATH"));
	if (!exec_path)
		return (strdup(""));
	return (exec_path);
}
