/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:27 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/10 20:42:04 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_pipeline(t_free_data *exec_data)
{
	int			pipefd[2];
	pid_t		pid;
	int			return_value;

	return_value = 0;
	exec_data->hd_fd = -1;
	if (exec_data->tree == NULL)
		return (0);
	if (exec_data->tree->sibling != NULL)
	{
		if (pipe(pipefd) == -1)
			return (print_err(1, 2, "my(s)hell: pipe\n"), 1);
	}
	if (is_there_here_doc(&exec_data->tree, &exec_data->hd_fd, exec_data))
		return (1); // deleted close(pipefd[0]),close(pipefd[1]),
	if (g_last_exit_status == 130)
		return (1); // deleted close(pipefd[0]),close(pipefd[1]),
	pid = fork();
	if (fork_check(pid, pipefd))
		return (1);
	else if (pid == 0)
		return_value = handle_child_process(pipefd, exec_data);
	else if (pid > 0)
		return_value = handle_parent_process(pipefd, pid, exec_data);
	return (return_value);
}

int	handle_child_process(int *pipefd, t_free_data *exec_data)
{
	if (exec_data->tree->sibling != NULL)
	{
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			print_err(1, 2, "my(s)hell: dup2\n");
			exit(EXIT_FAILURE);
		}
		close(pipefd[0]);
		close(pipefd[1]);
	}
	execute_node(exec_data);
	free_exit_data(exec_data);
	if (exec_data->hd_fd != -1)
		close(exec_data->hd_fd);
	close(STDOUT_FILENO);
	close(STDIN_FILENO);
	exit(g_last_exit_status);
}

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
		return_value = execute_pipeline(exec_data);
		free_exit_data(exec_data);
		close(STDIN_FILENO);
		exit(return_value);
	}
	close(pipefd[0]);
	return (pid2);
}
