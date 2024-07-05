/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_utils_1.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:21 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/05 11:31:08 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static int	handle_heredocs_parent(int *heredoc_pipefd, pid_t pid)
{
	int status;

	close(heredoc_pipefd[1]);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
	{
		if (dup2(heredoc_pipefd[0], STDIN_FILENO) == -1)
			return (print_err(1, 2, "my(s)hell: dup2 in sibling process\n"), 1);
	}
	else
		return (print_err(1, 2, "my(s)hell: error writing heredoc\n"), 1);
	close(heredoc_pipefd[0]);
	return 0;
}

int handle_heredocs(t_here_doc_data *here_docs)
{
	pid_t			pid;
	t_here_doc_data	*current_doc;
	int				heredoc_pipefd[2];

	if (here_docs == NULL)
		return 0;
	if (pipe(heredoc_pipefd) == -1)
		return (print_err(1, 2, "my(s)hell: fork in sibling process\n"), 1);
	pid = fork();
	if (pid == -1)
	{
		close(heredoc_pipefd[0]);
    	close(heredoc_pipefd[1]);
		return (print_err(1, 2, "my(s)hell: fork in sibling process\n"), 1);
	}
	if (pid == 0)
	{
		close(heredoc_pipefd[0]);
		current_doc = here_docs;
		while (current_doc != NULL)
		{
			write(heredoc_pipefd[1], current_doc->contents, strlen(current_doc->contents));
			current_doc = current_doc->next;
		}
		close(heredoc_pipefd[1]);
		exit(EXIT_SUCCESS);
	}
	else
		if (handle_heredocs_parent(heredoc_pipefd, pid))
			return (close(heredoc_pipefd[0]), 1);
	return (0);
}

int handle_child_process(int *pipefd, t_free_data *exec_data,
	t_here_doc_data *here_docs)
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
	if (handle_heredocs(here_docs) != 0)
		exit(EXIT_FAILURE);
	g_last_exit_status = execute_node(exec_data);
	exit(g_last_exit_status ? EXIT_FAILURE : EXIT_SUCCESS);
}

static int  ft_waitpid(int num_commands, pid_t *pids)
{
	int	i;
	int	status;

	i = 0;
	while (i < num_commands)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			g_last_exit_status = WEXITSTATUS(status);
		i++;
	}
	return (g_last_exit_status);
}

int handle_parent_process(int *pipefd, pid_t pid, t_free_data *exec_data,
						t_here_doc_data *here_docs)
{
	pid_t       pids [10];
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
		sibling_pid = handle_sibling_process(pipefd, &sibling_free_data, here_docs);
		if (sibling_pid == -1)
			return (close(pipefd[0]), 1);
		pids[num_commands] = sibling_pid;
		num_commands++;
	}
	ft_waitpid(num_commands, pids);
	return (g_last_exit_status);
}

int execute_pipeline(t_free_data *exec_data, t_here_doc_data *here_docs)
{
	int		pipefd[2];
	pid_t	pid;

	if (exec_data->tree == NULL)
		return 0;
	if (exec_data->tree->sibling != NULL)
	{
		if (pipe(pipefd) == -1)
			return (print_err(1, 2, "my(s)hell: pipe\n"), 1);
	}
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (print_err(1, 2, "my(s)hell: fork\n"), 1);
	}
	else if (pid == 0)
		return handle_child_process(pipefd, exec_data, here_docs);
	else
		return handle_parent_process(pipefd, pid, exec_data, here_docs);
	close(pipefd[0]);
	close(pipefd[1]);
}
