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
	execute_node(exec_data, here_docs);
	exit(g_last_exit_status);
}

/*static void ft_waitpid(int num_commands, pid_t *pids, int *final_status)
{
    int status;
    int exited_processes = 0;
    int i;

    *final_status = -1; // Initialize final status
	//printf("num_commands = %d\n", num_commands);
	printf("exited_processes = %d\n", exited_processes);
	printf("pids[0] = %d\n", pids[0]);
	printf("pids[1] = %d\n", pids[1]);
	printf("num_commands = %d\n", num_commands);
    while (exited_processes < num_commands)
    {
		i = 0;
        while (i < num_commands)
        {
            if (pids[i] > 0) // Ensure PID is valid
            {
                int wait_result = waitpid(pids[i], &status, WNOHANG);
                if (wait_result > 0)
                {
                    exited_processes++;
                    pids[i] = -1; // Mark this PID as handled
                    if (WIFEXITED(status))
                        *final_status = WEXITSTATUS(status);
                    else if (WIFSIGNALED(status))
                        *final_status = WTERMSIG(status) + 128;
                }
                else if (wait_result == -1 && errno != ECHILD)
                {
                    perror("waitpid error");
                    return; // Exit the function on error
                }
				//else if (wait_result == 0)
				//{
				//	printf("Child process %d is still running\n", pids[i]);
				//	continue ;
				//}
            }
			i++;
        }
    }
}
// The handle_parent_process function remains mostly unchanged.

int handle_parent_process(int *pipefd, pid_t pid, t_free_data *exec_data)
{
	pid_t       pids [10];
	int			final_status = 0;
	pid_t       sibling_pid;
	int         num_commands;
	t_free_data sibling_free_data;

	num_commands = 0;
	pids[num_commands] = pid;
	printf("pids[0] = %d\n", pids[0]);
	num_commands++;
	printf("im here again\n");
	if (exec_data->tree->sibling != NULL)
	{
		printf("going to sibling process AGAIN\n");
		close(pipefd[1]);
		sibling_free_data = *exec_data;
		sibling_free_data.tree = exec_data->tree->sibling->sibling;
		sibling_pid = handle_sibling_process(pipefd, &sibling_free_data);
		if (sibling_pid == -1)
			return (close(pipefd[0]), 1);
		pids[num_commands] = sibling_pid;
		num_commands++;
	}
	//printf("pids[0] = %d\n", pids[0]);
	//printf("pids[1] = %d\n", pids[1]);
	for (int i = 0; i < num_commands; i++) {
    printf("Final pids[%d] = %d\n", i, pids[i]);}
	ft_waitpid(num_commands, pids, &final_status);
	//printf("g_last_exit_status = %d\n", g_last_exit_status);
	g_last_exit_status = final_status;
	printf("g_last_exit_status 1 = %d\n", g_last_exit_status);
	return (g_last_exit_status);
}*/

static void ft_waitpid(int num_commands, pid_t *pids, int *statuses)
{
    int i;
    int status;

    i = 0;
    while (i < num_commands)
    {
        waitpid(pids[i], &status, 0);
        if (WIFEXITED(status))
            statuses[i] = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            statuses[i] = 128 + WTERMSIG(status);
        else
            statuses[i] = -1;  // Unhandled status case
        i++;
    }
}


int handle_parent_process(int *pipefd, pid_t pid, t_free_data *exec_data)
{
    pid_t pids[10];
    int statuses[10]; // Array to hold the statuses of each process
    pid_t sibling_pid;
    int num_commands;
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
    printf("Waiting for child processes to terminate...\n");
    ft_waitpid(num_commands, pids, statuses); // Capture exit statuses

    // Update global exit status with the last command's status
    g_last_exit_status = statuses[num_commands - 1];

    // Print the exit statuses in the order of creation
    for (int i = 0; i < num_commands; i++)
    {
        printf("Exit status of command %d (PID %d): %d\n", i, pids[i], statuses[i]);
    }

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
	printf("Attempting to fork pipeline...\n");
	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (print_err(1, 2, "my(s)hell: fork\n"), 1);
	}
	else if (pid == 0)
	{
		printf("Child process PID: %d\n", getpid());
		return_value = handle_child_process(pipefd, exec_data, here_docs);
	}
	else if (pid > 0)
	{
		printf("Parent PID: %d, Child PID pipeline: %d\n", getpid(), pid);
		return_value = handle_parent_process(pipefd, pid, exec_data);
		printf("Waiting for child process to terminate pipeline...\n");
	}
	if (here_docs != NULL)
		free(here_docs);
	//close(pipefd[0]);
	//close(pipefd[1]);
	return (return_value);
}

