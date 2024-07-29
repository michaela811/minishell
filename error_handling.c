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

/* int	error_message(char *str)
{
	perror(str);
	return (1);
} */

/* void	execve_error(char **s_cmd)
{
	print_err(138, 2, "Execve failed\n");
	ft_putendl_fd("This command was not found: ", 2);
	ft_putendl_fd(s_cmd[0], 2);
	free_array(s_cmd);
	exit(0);
} */

void	ft_delay(unsigned int microseconds)
{
	volatile unsigned int	counter;
	unsigned int			delay_count;

	counter = 0;
	delay_count = microseconds * 10;
	while (counter < delay_count)
		counter++;
}

int	ft_flock(const char *lockfile)
{
	int	fd;

	while ((fd = open(lockfile, O_CREAT | O_EXCL, 0644)) == -1)
	{
		if (errno != EEXIST)
		{
			perror("open");
			return -1;
		}
		ft_delay(10000);
	}
	close(fd);
	return (0);
}

int	ft_funlock(const char *lockfile)
{
	return (unlink(lockfile));
}

void	print_err(int status, unsigned int fd, char *format, ...)
{
	va_list		args;
	va_list		args_copy;
	const char	*lockfile;
	
	lockfile = "/tmp/my_lockfile.lock";
	va_start(args, format);
	va_copy(args_copy, args);
	g_last_exit_status = status;
	if (ft_flock(lockfile) == 0)
	{
		ft_vprintf_fd(fd, format, args_copy);
		ft_funlock(lockfile);
	}
	va_end(args_copy);
	va_end(args);
}
