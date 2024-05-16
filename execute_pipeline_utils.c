#include "lexer.h"

int	handle_child_process(int *pipefd, t_free_data *free_data)
{
	int	return_value;

	if (free_data->tree->sibling != NULL)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	return_value = execute_node(free_data);
	if (return_value != 0)
	{
		g_last_exit_status = return_value;
		exit(EXIT_FAILURE);
	}
	g_last_exit_status = 0;
	exit(EXIT_SUCCESS);
}

int	handle_sibling_process(int *pipefd, t_free_data *free_data)
{
	pid_t	pid2;
	int		return_value;

	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
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
		wait(NULL);
	}
	else
		return (perror("fork"), 1);
	//free_env(*env);
	return (0);
}

int	handle_parent_process(int *pipefd, pid_t pid, t_free_data *free_data)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && (WEXITSTATUS(status) != EXIT_SUCCESS))
	{
		g_last_exit_status = WEXITSTATUS(status);
		return (perror("Child process failed"), 1);
	}
	if (free_data->tree->sibling != NULL)
	{
		close(pipefd[1]);
		if (handle_sibling_process(pipefd, free_data))
			return (1);
	}
	wait(NULL);
	g_last_exit_status = 0;
	return (0);
}
