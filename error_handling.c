/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:33:30 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 10:33:32 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/* int ft_flock(const char *lockfile)
{
	int				fd;
	struct flock	fl;

	fd = open(lockfile, O_RDWR | O_CREAT, 0644);
	if (fd == -1)
		return (perror("my(s)hell: open"), -1);
	ft_memset(&fl, 0, sizeof(fl));
	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;
	if (fcntl(fd, F_SETLK, &fl) == -1)
	{
		if (errno == EACCES || errno == EAGAIN)
			printf("my(s)hell: resource is locked by another process.\n");
		else
			perror("fcntl");
		close(fd);
		return (-1);
	}
	return (fd);
}

void ft_funlock(int fd)
{
	struct flock fl;

	ft_memset(&fl, 0, sizeof(fl));
	fl.l_type = F_UNLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 0;
	if (fcntl(fd, F_SETLK, &fl) == -1)
		perror("my(s)hell: fcntl");
	close(fd);
} */

int ft_flock(const char *lockfile)
{
    int fd;

    // Attempt to create the lock file exclusively
    fd = open(lockfile, O_RDWR | O_CREAT | O_EXCL, 0644);
    if (fd == -1)
    {
        if (errno == EEXIST)
        {
            printf("my(s)hell: resource is locked by another process.\n");
        }
        else
        {
            perror("my(s)hell: open");
        }
        return -1;
    }

    return fd;
}

void ft_funlock(int fd, const char *lockfile)
{
    // Close the lock file descriptor and remove the lock file
    close(fd);
    if (unlink(lockfile) == -1)
    {
        perror("my(s)hell: unlink");
    }
}

/* void print_err(int status, unsigned int fd, char *format, ...)
{
	va_list		args;
	va_list		args_copy;
	const char	*lockfile;
	int			lock_fd;

	lockfile = "/tmp/my_lockfile.lock";
	va_start(args, format);
	va_copy(args_copy, args);
	g_last_exit_status = status;
	lock_fd = ft_flock(lockfile);
	if (lock_fd >= 0)
	{
		ft_vprintf_fd(fd, format, args_copy);
		ft_funlock(lock_fd, lockfile);
	}
	va_end(args_copy);
	va_end(args);
} */

void	print_err(int status, unsigned int fd, char *format, ...)
{
	va_list	args;
	va_list	args_copy;

	va_start(args, format);
	va_copy(args_copy, args);
	g_last_exit_status = status;
	ft_vprintf_fd(fd, format, args_copy);
	va_end(args_copy);
	va_end(args);
}

void	print_echo(int status, unsigned int fd, char *format, ...)
{
	va_list		args;
	va_list		args_copy;

	va_start(args, format);
	va_copy(args_copy, args);
	g_last_exit_status = status;
	ft_vprintf_fd(fd, format, args_copy);
	va_end(args_copy);
	va_end(args);
}
