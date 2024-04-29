#include "lexer.h"

int	execute_command(t_exec_vars *vars, t_env **env)
{
	char	**environment;
	int		return_builtins;

	environment = env_list_to_array(*env);
	if (environment == NULL)
	{
		g_last_exit_status = 1;
		return (1);
	}
	return_builtins = exec_builtins(vars->args, env, environment);
	if (return_builtins == 2)
	{
		if (handle_fork(vars, env, environment))
		{
			g_last_exit_status = 1;
			return (free_env_array(environment), 1);
		}
	}
	else if (return_builtins == 1)
	{
		g_last_exit_status = 1;
		return (free_env_array(environment), 1);
	}
	g_last_exit_status = 0;
	return (free_env_array(environment), 0);
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
	printf("Path: %s\n", path);

printf("Args: ");
for (int i = 0; vars->args[i] != NULL; i++)
{
    printf("%s ", vars->args[i]);
}
printf("\n");

printf("Environment: ");
for (int i = 0; environment[i] != NULL; i++)
{
    printf("%s ", environment[i]);
}
printf("\n");

if (execve(path, vars->args, environment) < 0)
{
    g_last_exit_status = 127;
    perror("execve");
    return (1);//exit(EXIT_FAILURE);
}
	if (execve(path, vars->args, environment) < 0)
	{
		printf ("exited in execve\n");
		g_last_exit_status = 127;
		perror("execve");
		return (1);//exit(EXIT_FAILURE);
	}
	printf ("exited after execve\n");
	g_last_exit_status = 0;
	return (0);
}

int	handle_fork(t_exec_vars *vars, t_env **env, char **environment)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		g_last_exit_status = 128;
		return (1);//exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (handle_child_cmd(vars, env, environment))
		{
			printf ("was an error");
			g_last_exit_status = 155;
			return (1);
		}
		printf ("no error");
	}
	else
	{
		waitpid(pid, &status, 0);
		//g_last_exit_status = WEXITSTATUS(status);
		g_last_exit_status = 133;
	}
	return (0);
}
