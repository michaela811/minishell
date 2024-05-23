#include "minishell.h"


int	handle_child_process(int *pipefd, t_free_data *exec_data)
{
	//int	return_value;
	//printf("Entering handle_child_process function\n");

	if (exec_data->tree->sibling != NULL)
	{
		close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
		close(pipefd[1]);
	}
	//return_value = execute_node(exec_data);
	if (execute_node(exec_data))
	{
		//g_last_exit_status = return_value;
		exit(EXIT_FAILURE);
		//exit(g_last_exit_status);
	}
	g_last_exit_status = 0;
	//printf("Exiting handle_child_process function with success\n");
	exit(EXIT_SUCCESS);
	//exit(g_last_exit_status);
}

pid_t handle_sibling_process(int *pipefd, t_free_data *exec_data)
{
    pid_t	pid2;
    int		return_value;

	//printf("Entering handle_sibling_process function\n");
    pid2 = fork();
	if (pid2 == -1)
		return (printf_global_error(1, 2, "my(s)hell: fork in sibling process\n"), 1);
    if (pid2 == 0)
    {
        if (dup2(pipefd[0], STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(pipefd[0]);
		close(pipefd[1]);//New trial
        return_value = execute_pipeline(exec_data);
        if (return_value != 0)
        {
            g_last_exit_status = return_value;
            exit(EXIT_FAILURE);
        }
        g_last_exit_status = 0;
		//printf("Exiting handle_sibling_process function with success\n");
        exit(EXIT_SUCCESS);
    }
    else if (pid2 > 0)
    {
        close(pipefd[0]);
		close(pipefd[1]);//New trial
        wait(NULL);
    }
    return pid2;
}

int	handle_parent_process(int *pipefd, pid_t pid, t_free_data *exec_data)
{
	int	status;
	pid_t pids [10];
	int num_commands = 0;

	pids[num_commands] = pid;
	num_commands++;
	if (exec_data->tree->sibling != NULL)
	{
		close(pipefd[1]);
		t_free_data sibling_free_data = *exec_data;
    	sibling_free_data.tree = exec_data->tree->sibling->sibling;
		pid_t sibling_pid = handle_sibling_process(pipefd, &sibling_free_data);
		if (sibling_pid == -1)
			return (1);
		pids[num_commands] = sibling_pid;
	num_commands++;
	}
	for (int i = 0; i < num_commands; i++)
	{
    waitpid(pids[i], &status, 0);
    if (WIFEXITED(status))
        g_last_exit_status = WEXITSTATUS(status);
	}
	return g_last_exit_status;
}