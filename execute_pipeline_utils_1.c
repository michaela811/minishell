/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_utils_1.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:21 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/24 15:03:53 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

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

static void	ft_waitpid(int num_commands, pid_t *pids, int *statuses)
{
	int	status;

	status = 0;
	num_commands--;
	while (num_commands >= 0)
	{
		waitpid(pids[num_commands], &status, 0);
		if (WIFEXITED(status))
			statuses[num_commands] = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			statuses[num_commands] = 128 + WTERMSIG(status);
		else
			statuses[num_commands] = -1;
		num_commands--;
	}
}

int	handle_parent_process(int *pipefd, pid_t pid, t_free_data *exec_data)
{
	pid_t		pids[10];
	int			statuses[10];
	pid_t		sibling_pid;
	int			num_commands;
	t_free_data	sibling_free_data;

	num_commands = 0;
	pids[num_commands] = pid;
	num_commands++;
	if (exec_data->tree->sibling != NULL)
	{
		close(pipefd[1]);
		sibling_free_data = *exec_data;
		sibling_free_data.tree = exec_data->tree->sibling->sibling;
		sibling_pid = handle_sibling_process(pipefd, &sibling_free_data);
		if (sibling_pid == -1)
			return (close(pipefd[0]), 1);
		pids[num_commands] = sibling_pid;
		num_commands++;
	}
	ft_waitpid(num_commands, pids, statuses);
	g_last_exit_status = statuses[num_commands - 1];
	return (g_last_exit_status);
}

static int	fork_check(pid_t pid, int *pipefd)
{
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		print_err(1, 2, "my(s)hell: fork\n");
		return (1);
	}
	else
		return (0);
}

int	execute_pipeline(t_free_data *exec_data)
{
	int			pipefd[2];
	pid_t		pid;
	int			return_value;

	return_value = 0;
	if (exec_data->tree == NULL)
		return (0);
	if (exec_data->tree->sibling != NULL)
	{
		if (pipe(pipefd) == -1)
			return (print_err(1, 2, "my(s)hell: pipe\n"), 1);
	}
	is_there_here_doc(&exec_data->tree, &exec_data->hd_fd);
	pid = fork();
	if (fork_check(pid, pipefd))
		return (1);
	else if (pid == 0)
		return_value = handle_child_process(pipefd, exec_data);
	else if (pid > 0)
		return_value = handle_parent_process(pipefd, pid, exec_data);
	return (return_value);
}
