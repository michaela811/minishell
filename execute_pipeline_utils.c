#include "lexer.h"

int	handle_child_process(t_parse_tree *node, t_env **env, int *pipefd)
{
	int	return_value;

	if (node->sibling != NULL)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
	}
	return_value = execute_node(node, env);
	if (return_value != 0)
	{
		g_last_exit_status = return_value;
		return (1);//exit(EXIT_FAILURE);
	}
	g_last_exit_status = 0;
	return (0);//exit(EXIT_SUCCESS);
}

int	handle_sibling_process(t_parse_tree *node, t_env **env, int *pipefd)
{
	pid_t	pid2;
	int		return_value;

	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		return_value = execute_pipeline(node->sibling->sibling, env);
		if (return_value != 0)
		{
			g_last_exit_status = return_value;
			return (1);//exit(EXIT_FAILURE);
		}
		g_last_exit_status = 0;
		return (0);//exit(EXIT_SUCCESS);
	}
	else if (pid2 > 0)
	{
		close(pipefd[0]);
		wait(NULL);
	}
	else
		return (perror("fork"), 1);
	return (0);
}

int	handle_parent_process(t_parse_tree *node, t_env **env,
	int *pipefd, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) != EXIT_SUCCESS)
	{
		g_last_exit_status = WEXITSTATUS(status);
		return (perror("Child process failed"), 1);
	}
	if (node->sibling != NULL)
	{
		close(pipefd[1]);
		if (handle_sibling_process(node, env, pipefd))
			return (1);
	}
	wait(NULL);
	g_last_exit_status = 0;
	return (0);
}
