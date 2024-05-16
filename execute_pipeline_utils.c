#include "minishell.h"


int	handle_child_process(int *pipefd, t_free_data *free_data)
{
	//int	return_value;

	if (free_data->tree->sibling != NULL)
	{
		close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
		close(pipefd[1]);
	}
	//return_value = execute_node(node, env);
	if (execute_node(free_data))
	{
		//g_last_exit_status = return_value;
		//exit(EXIT_FAILURE);
		exit(g_last_exit_status);
	}
	//g_last_exit_status = 0;
	//exit(EXIT_SUCCESS);
	exit(g_last_exit_status);
}

pid_t handle_sibling_process(int *pipefd, t_free_data *free_data)
{
    pid_t	pid2;
    int		return_value;

    pid2 = fork();
    if (pid2 == 0)
    {
        if (dup2(pipefd[0], STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(pipefd[0]);
		close(pipefd[1]);//New trial
        return_value = execute_pipeline(free_data);
        if (return_value != 0)
        {
            g_last_exit_status = return_value;
            exit(EXIT_FAILURE);
        }
        g_last_exit_status = 0;
        exit(EXIT_SUCCESS);
    }
    else if (pid2 > 0)
    {
        close(pipefd[0]);
		close(pipefd[1]);//New trial
        wait(NULL);
    }
    else
    {
        perror("fork");
        return -1;
    }
    return pid2;
}

/* int	handle_sibling_process(t_parse_tree **node, t_env **env, int *pipefd)
{
	pid_t	pid2;
	int		return_value;

	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		return_value = execute_pipeline(&(*node)->sibling->sibling, env);
		if (return_value != 0)
		{
			g_last_exit_status = return_value;
			exit(EXIT_FAILURE);
		}
		g_last_exit_status = 0;
		exit(EXIT_SUCCESS);
	}
	else if (pid2 > 0)
	{
		close(pipefd[0]);
		wait(NULL);
	}
	else
		return (perror("fork"), 1);
	//free_env(*env);
	return (0);
} */

/* int	handle_parent_process(t_parse_tree **node, t_env **env,
	int *pipefd, pid_t pid)
{

	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && (WEXITSTATUS(status) != EXIT_SUCCESS))
	{
		g_last_exit_status = WEXITSTATUS(status);
		return (perror("Child process failed"), 1);
	}
	if ((*node)->sibling != NULL)
	{
		close(pipefd[1]);
		if (handle_sibling_process(pipefd, free_data))
			return (1);
	}
	wait(NULL);
	g_last_exit_status = 0;
	return (0);
} */

int	handle_parent_process(int *pipefd, pid_t pid, t_free_data *free_data)
{
	int	status;
	pid_t pids [10];//[MAX_COMMANDS];  // Array to store PIDs of child processes
	int num_commands = 0;  // Number of commands in the pipeline

	pids[num_commands] = pid;
	num_commands++;

	// Handle sibling commands in the pipeline
	if (free_data->tree->sibling != NULL)
	{
		close(pipefd[1]);  // Close the unused write end of the pipe
		pid_t sibling_pid = handle_sibling_process(pipefd, free_data);
		if (sibling_pid == -1)
			return (1); // Early exit on error in handling sibling, may want to adjust depending on desired behavior
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