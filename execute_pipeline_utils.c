/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:27 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/24 15:02:21 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	handle_sibling_process(int *pipefd, t_free_data *exec_data)
{
	pid_t	pid2;
	int		return_value;

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
		if (exec_data->hd_fd != -1)
			close(exec_data->hd_fd);
		//close(STDIN_FILENO);	
		return_value = execute_pipeline(exec_data);
		free_exit_data(exec_data);
		close(STDIN_FILENO);
		exit(return_value);
	}
	close(pipefd[0]);
	return (pid2);
}
