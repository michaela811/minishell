#include "lexer.h"
#include <errno.h>

int	execute_command(t_exec_vars *vars, t_env **env, t_token_list *token_list, t_parse_tree *root)
{
	char	**environment;
	int		return_builtins;

	environment = env_list_to_array(*env);
	if (environment == NULL)
	{
		g_last_exit_status = 1;
		return (free_env(*env), 1);
	}
	return_builtins = exec_builtins(vars, env, environment, token_list, root);
	if (return_builtins == 2)
	{
		if (handle_fork(vars, env, environment))
		{
			g_last_exit_status = 1;
			return (free_env(*env), free_env_array(environment), 1);
		}
	}
	else if (return_builtins == 1)
	{
		g_last_exit_status = 1;
		return (free_env(*env), free_env_array(environment), 1);
	}
	g_last_exit_status = 0;
	return (free_env(*env), free_env_array(environment), 0);
}

int	handle_child_cmd(t_exec_vars *vars, t_env **env, char **environment)
{
	char	*path;

	if (vars->fd_in != 0)
	{
		dup2(vars->fd_in, 0);
		close(vars->fd_in);
	}
	if (vars->fd_out != 1)
	{
		dup2(vars->fd_out, 1);
		close(vars->fd_out);
	}
	if (get_path(vars->args[0], *env, &path))
	{
		g_last_exit_status = 138;
		return (1);
	}
	if (execve(path, vars->args, environment) < 0)
	{
		g_last_exit_status = 127;
		perror("execve");
		exit(EXIT_FAILURE);
	}
	g_last_exit_status = 0;
	exit(EXIT_SUCCESS);
}

int	handle_fork(t_exec_vars *vars, t_env **env, char **environment)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		free_env_array(environment);
		perror("fork");
		g_last_exit_status = 128;
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (handle_child_cmd(vars, env, environment))
		{
			free_env_array(environment);
			g_last_exit_status = 155;
			return (1);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		g_last_exit_status = WEXITSTATUS(status);
	}
	free_env_array(environment);
	return (0);
}
