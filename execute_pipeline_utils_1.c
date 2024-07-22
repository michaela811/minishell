/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_utils_1.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:21 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/18 17:44:56 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int handle_child_process(int *pipefd, t_free_data *exec_data, t_hd_data *here_docs)
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
	//g_last_exit_status = execute_node(exec_data, here_docs);
	execute_node(exec_data, here_docs);
	exit(g_last_exit_status);
}

static void	ft_waitpid(int num_commands, pid_t *pids, int *statuses)
{
	int	i;
	int	status;

	i = 0;
	while (i < num_commands)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			statuses[i] = WEXITSTATUS(status);
		i++;
	}
}

int handle_parent_process(int *pipefd, pid_t pid, t_free_data *exec_data)
{
	pid_t       pids [10];
	int			statuses[10];
	pid_t       sibling_pid;
	int         num_commands;
	t_free_data sibling_free_data;

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
	ft_memset(statuses, 0, sizeof(statuses[0] * num_commands));
	ft_waitpid(num_commands, pids, statuses);
	g_last_exit_status = statuses[num_commands - 1];
	return (g_last_exit_status);
}

int execute_pipeline(t_free_data *exec_data)
{
	int			pipefd[2];
	pid_t		pid;
	t_hd_data	*here_docs;
	int			return_value;

	here_docs = NULL;
	if (exec_data->tree == NULL)
		return (0);
	if (exec_data->tree->sibling != NULL)
	{
		if (pipe(pipefd) == -1)
			return (print_err(1, 2, "my(s)hell: pipe\n"), 1);
	}
	is_there_here_doc(&exec_data->tree, &here_docs);
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (print_err(1, 2, "my(s)hell: fork\n"), 1);
	}
	else if (pid == 0)
		return_value = handle_child_process(pipefd, exec_data, here_docs);
	else
		return_value = handle_parent_process(pipefd, pid, exec_data);
	if (here_docs != NULL)
		free(here_docs);
	//close(pipefd[0]);
	//close(pipefd[1]);
	printf("return_value: %d\n", return_value);
	return (return_value);
}

