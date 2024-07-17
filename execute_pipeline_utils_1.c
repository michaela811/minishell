/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_utils_1.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:21 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/16 14:27:55 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

/*int handle_child_process(int *pipefd, t_free_data *exec_data, t_hd_data *here_docs)
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
	if ((g_last_exit_status = execute_node(exec_data, here_docs)))
		exit(EXIT_FAILURE);
	exit(EXIT_SUCCESS);
}*/

int handle_child_process(int *pipefd, t_free_data *exec_data, t_hd_data *here_docs)
{
    pid_t child_pid;
    int status;

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

    // Assuming execute_node now returns the PID of a child process it spawns
    child_pid = execute_node(exec_data, here_docs);

    if (child_pid > 0) {
        // Wait for the child process to change state
        waitpid(child_pid, &status, 0);

        // Check if the child process exited normally
        if (WIFEXITED(status)) {
            // Get the exit status of the child process
            g_last_exit_status = WEXITSTATUS(status);
        } else {
            // Handle other termination scenarios (e.g., signal termination)
            g_last_exit_status = -1; // Example error code
        }
    } else {
        // execute_node failed to create a child process
        exit(EXIT_FAILURE);
    }

    exit(g_last_exit_status); // Use the captured exit status
}

/*static int  ft_waitpid(int num_commands, pid_t *pids, int *status)
{
	int	i;

	i = 0;
	while (i < num_commands)
	{
		waitpid(pids[i], status, 0);
		if (WIFEXITED(*status))
			g_last_exit_status = WEXITSTATUS(*status);
		else if (WIFSIGNALED(*status))
            g_last_exit_status = 128 + WTERMSIG(*status);
		i++;
	}
	return (g_last_exit_status);
}*/

static int ft_waitpid(int num_commands, pid_t *pids)
{
    int i;
    int command_status;
	//int exit_status;
    int any_fail;

    i = 0;
	any_fail = 0;
    while (i < num_commands)
    {
        waitpid(pids[i], &command_status, 0);
        if (WIFEXITED(command_status))
		{
            g_last_exit_status = WEXITSTATUS(command_status);
            if (g_last_exit_status != 0)
			{
                //g_last_exit_status = exit_status;
                any_fail = 1; // Mark that we've encountered a failure
            }
        }
		else if (WIFSIGNALED(command_status))
		{
            //g_last_exit_status = 128 + WTERMSIG(command_status);
            g_last_exit_status = WEXITSTATUS(command_status);
			any_fail = 1; // Mark that we've encountered a failure
        }
        i++;
    }
    if (!any_fail) {
        // If no command failed, ensure g_last_exit_status is 0
        g_last_exit_status = 0;
    }
    return (g_last_exit_status);
}

int handle_parent_process(int *pipefd, pid_t pid, t_free_data *exec_data)
{
	pid_t       pids [10];
	//int			status;
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
	g_last_exit_status = ft_waitpid(num_commands, pids);//, &status);
	return (g_last_exit_status);
}

int execute_pipeline(t_free_data *exec_data)
{
	int		pipefd[2];
	pid_t	pid;
	t_hd_data *here_docs;

	here_docs = NULL;
	if (exec_data->tree == NULL)
		return 0;
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
		return handle_child_process(pipefd, exec_data, here_docs);
	else
		return handle_parent_process(pipefd, pid, exec_data);
	close(pipefd[0]);
	close(pipefd[1]);
}

