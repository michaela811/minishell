#include "minishell.h"

int	handle_child_process(int *pipefd, t_free_data *exec_data)
{
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
	if ((g_last_exit_status = execute_node(exec_data)))
		exit(EXIT_FAILURE);
	g_last_exit_status = 0;
	exit(EXIT_SUCCESS);
}

static int	check_return_value(int return_value)
{
	if (return_value != 0)
	{
		g_last_exit_status = return_value;
		return (EXIT_FAILURE);
	}
	g_last_exit_status = 0;
	return (0);
}

static void	close_fd(int *pipefd)
{
	close(pipefd[0]);
	close(pipefd[1]);
}

static int	pid2_check(pid_t pid2)
{
	if (pid2 == -1)
		return (printf_global_error(1, 2,
				"my(s)hell: fork in sibling process\n"), 1);
	else
		return (0);
}

pid_t	handle_sibling_process(int *pipefd, t_free_data *exec_data)
{
	pid_t	pid2;
	int		return_value;

	pid2 = fork();
	if (pid2_check(pid2))
		return (g_last_exit_status);
	if (pid2 == 0)
	{
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
		close_fd(pipefd);
		return_value = execute_pipeline(exec_data);
		if (check_return_value(return_value))
			exit (EXIT_FAILURE);
		exit(EXIT_SUCCESS);
	}
	else if (pid2 > 0)
	{
		close_fd(pipefd);
		wait(NULL);
	}
	return (pid2);
}

static int	ft_waitpid(int num_commands, pid_t *pids)
{
	int	i;
	int	status;

	i = 0;
	while (i++ < num_commands)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
		{
			g_last_exit_status = WEXITSTATUS(status);
			return (g_last_exit_status);
		}
	}
	return (g_last_exit_status);
}

int	handle_parent_process(int *pipefd, pid_t pid, t_free_data *exec_data)
{
	pid_t		pids [10];
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
			return (1);
		pids[num_commands] = sibling_pid;
		num_commands++;
	}
	ft_waitpid(num_commands, pids);
	return (g_last_exit_status);
}
