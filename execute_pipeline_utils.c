/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:27 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 10:36:29 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_return_value(int return_value)
{
	if (return_value != 0)
	{
		g_last_exit_status = return_value;
		return (EXIT_FAILURE);
	}
	g_last_exit_status = 0;
	return (0);
}

static void	close_fd(int *pipefd)
{
	close(pipefd[0]);
	close(pipefd[1]);
}

static int	pid2_check(pid_t pid2)
{
	if (pid2 == -1)
		return (print_err(1, 2,
				"my(s)hell: fork in sibling process\n"), 1);
	else
		return (0);
}

pid_t	handle_sibling_process(int *pipefd, t_free_data *exec_data)
{
	pid_t	pid2;
	int		return_value;

	pid2 = fork();
	if (pid2_check(pid2))
		return (g_last_exit_status);
	if (pid2 == 0)
	{
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close_fd(pipefd);
		return_value = execute_pipeline(exec_data);
		if (check_return_value(return_value))
			exit (EXIT_FAILURE);
		exit(EXIT_SUCCESS);
	}
	else if (pid2 > 0)
	{
		close_fd(pipefd);
		wait(NULL);
	}
	return (pid2);
}
