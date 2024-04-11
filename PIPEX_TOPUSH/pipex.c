/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 10:35:01 by mmasarov          #+#    #+#             */
/*   Updated: 2024/01/30 10:35:05 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **env)
{
	pid_t	pipex;
	int		fd[2];

	if (access(argv[1], F_OK == -1))
		exit_function(2);
	if (argc != 5)
		exit_function(1);
	if (pipe(fd) == -1)
		exit(-1);
	//printf("these fd are openbefore fork: fd - %d and fd - %d\n", fd[0], fd[1]);
	fflush(stdout);
	pipex = fork();
	printf("these fd are open: fd - %d and fd - %d\n", fd[0], fd[1]);
	printf("Im parent process my PID %d and Im child process PID %d\n", getpid(), pipex);
	fflush(stdout);
	if (pipex < 0)
		exit(-1);
	else if (pipex == 0)
		child(argv, fd, env);
	else
	{
		parent(argv, fd, env);
		waitpid(pipex, NULL, 0);
	}
	printf("these fd are open: fd - %d and fd - %d\n", fd[0], fd[1]);
	fflush(stdout);
	return (0);
}
