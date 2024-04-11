/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 15:40:36 by mmasarov          #+#    #+#             */
/*   Updated: 2024/01/30 17:20:27 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	execute(char *cmd, char **env)
{
	char	**s_cmd;
	char	*path;

	s_cmd = ft_split(cmd, ' ');
	if (s_cmd == NULL)
		error_message("Malloc error in split function: ");
	path = get_path(s_cmd[0], env);
	if (execve(path, s_cmd, env) == -1)
		execve_error(s_cmd);
}

void	child(char **argv, int *p_fd, char **env)
{
	int	fd;

	fd = open_file(argv[1], 0);
	//int saved_stdout = open("/dev/tty", O_WRONLY);
	//printf("These fd are open in child: fd - %d and p_fd[0] %d, p_fd[1] %d\n", fd, p_fd[0], p_fd[1]);
	dup2(fd, 0);
	//printf("These fd are open in child after dup2 first: fd - %d and p_fd[0] %d, p_fd[1] %d\n", fd, p_fd[0], p_fd[1]);
	dup2(p_fd[1], 1);
	//dprintf(saved_stdout, "These fd are open in child after dup2 second: fd - %d and p_fd[0] %d, p_fd[1] %d\n", fd, p_fd[0], p_fd[1]);
	close(p_fd[0]);
	close(fd);
	execute(argv[2], env);
}

void	parent(char **argv, int *p_fd, char **env)
{
	int	fd;

	fd = open_file(argv[4], 1);
	dup2(fd, 1);
	dup2(p_fd[0], 0);
	close(p_fd[1]);
	close(fd);
	execute(argv[3], env);
}
