#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include "module_debug.h"
#include "ft_std.h"
#include "utils.h"
#include <stdio.h>

/**
 * @brief Vide le logger.
 * @param lg Le logger à vider.
 */
void	trace_logger_flush(int fd, t_logger *lg, bool to_file)
{
	if (lg->pos && to_file == false)
		write(fd, lg->buf, lg->pos);
	if (lg->pos > 0 && to_file == true)
		debug_log(lg->buf);
}

void	debug_log(const char *msg)
{
	int				fd;
	struct timeval	tv;
	struct tm		*t;
	char			time_str[20];
	int				len;

	if (DLVL < 0)
		return ;
	fd = open("/dev/shm/sh42_dbg/.trace.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return ;

	gettimeofday(&tv, NULL);
	t = localtime(&tv.tv_sec);
	if (!t)
		return ;

	// Format : [HH:MM:SS.mmm]
	time_str[0] = '[';
	time_str[1] = '0' + (t->tm_hour / 10);
	time_str[2] = '0' + (t->tm_hour % 10);
	time_str[3] = ':';
	time_str[4] = '0' + (t->tm_min / 10);
	time_str[5] = '0' + (t->tm_min % 10);
	time_str[6] = ':';
	time_str[7] = '0' + (t->tm_sec / 10);
	time_str[8] = '0' + (t->tm_sec % 10);
	time_str[9] = '.';
	time_str[10] = '0' + ((tv.tv_usec / 100000) % 10);
	time_str[11] = '0' + ((tv.tv_usec / 10000) % 10);
	time_str[12] = '0' + ((tv.tv_usec / 1000) % 10);
	time_str[13] = ']';
	time_str[14] = ' ';
	time_str[15] = '\0';

	//write(fd, time_str, 15);
	(void)time_str;
	len = 0;
	while (msg[len])
		len++;
	write(fd, msg, len);
	//write(fd, "\n", 1);
	close(fd);
}

void	debug_log_init(void)
{
	const char *dir  = "/dev/shm/sh42_dbg";
	const char *file = "/dev/shm/sh42_dbg/.trace.log";
	int				fd;
	struct timeval	tv;
	struct tm		*t;
	char			buf[64];
	int				len;

	if (DLVL < 0)
		return ;
	if (mkdir(dir, 0755) < 0 && errno != EEXIST)
		return ;
	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return ;

	gettimeofday(&tv, NULL);
	t = localtime(&tv.tv_sec);
	if (!t)
	{
		close(fd);
		return ;
	}
	len = snprintf(buf, sizeof(buf),
		"\n\n---- NEW SESSION [%02d:%02d:%02d.%03ld] ----\n",
		t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec / 1000);
	write(fd, buf, len);
	close(fd);
}
