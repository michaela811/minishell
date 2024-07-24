/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:27 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/24 14:51:30 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	handle_sibling_process(int *pipefd, t_free_data *exec_data)
{
	pid_t	pid2;

	pid2 = fork();
	if (pid2 == -1)
		return (print_err(errno, 2, "my(s)hell: fork"), -1);
	if (pid2 == 0)
	{
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			print_err(errno, 2, "my(s)hell: dup2");
			exit(EXIT_FAILURE);
		}
		close(pipefd[0]);
		close(pipefd[1]);
		execute_pipeline(exec_data);
		exit(EXIT_SUCCESS);
	}
	/*else if (pid2 > 0)
	{
		int status;
		close(pipefd[0]);
		close(pipefd[1]);
		//if (close(pipefd[1]) == -1)
		//    perror("my(s)hell: close write end of pipe");
		waitpid(pid2, &status, WNOHANG);
		if (WIFEXITED(status))
			g_last_exit_status = WEXITSTATUS(status);
		else
			g_last_exit_status = -1;
	}*/
	close(pipefd[0]);
	close(pipefd[1]);
	return (pid2);
}
