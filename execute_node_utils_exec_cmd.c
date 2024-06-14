#include "minishell.h"
#include <errno.h>

int	execute_command(t_exec_vars *vars, t_free_data *exec_data)
{
	int		return_builtins;

	if (!vars->args[0])
		return (0);
	exec_data->environment = env_list_to_array(exec_data->env);
	if (exec_data->environment == NULL)
	{
		g_last_exit_status = 1;
		return (free_command_data(exec_data), g_last_exit_status);
	}
	return_builtins = exec_builtins(vars, exec_data);
	if (return_builtins == 3)
		handle_fork(vars, &exec_data->env, exec_data->environment);
	return (g_last_exit_status);
}

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int is_directory(const char *path)
{
    struct stat path_stat;

    if (stat(path, &path_stat) == -1)
        return -1;
    return S_ISDIR(path_stat.st_mode);
}

int get_args_count(char **args)
{
    int count = 0;
    while (args[count] != NULL)
        count++;
    return (count);
}

int	handle_child_cmd(t_exec_vars *vars, t_env **env, char **environment)
{
	char	*path;
	int		path_status;

	//process_args(vars->args, &vars->error);
	//if (vars->error)
		//exit (g_last_exit_status);
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
	if (ft_strchr(vars->args[0], '/'))
	{
	    int dir_check = is_directory(vars->args[0]);
	    if (dir_check == -1)
			return (printf_global_error(127, 2, "my(s)hell: %s: No such file or directory\n", vars->args[0]), 127);
		else if (dir_check)
			return (printf_global_error(126, 2, "my(s)hell: %s: Is a directory\n", vars->args[0]), 126);
	}
	if (access(vars->args[0], F_OK | X_OK) == 0 && vars->args[0][0] == '/')
	{
		path = vars->args[0];
		path_status = 0;
	}
	else
		path_status = get_path(vars->args[0], *env, &path);
	if (path_status == 1)
		exit (g_last_exit_status);
	if (path_status == -1)
	{
		if (access(vars->args[0], X_OK) == -1 && vars->args[0][0] == '.' && vars->args[0][1] == '/')
		{
			printf_global_error(126, 2, "my(s)hell: %s: Permission denied\n", vars->args[0]);
			if (vars->fd_in != 0) close(vars->fd_in);
            if (vars->fd_out != 1) close(vars->fd_out);
			exit(126);
		}
		printf_global_error(127, 2, "my(s)hell: %s: command not found\n", vars->args[0]);
		if (vars->fd_in != 0) close(vars->fd_in);
    	if (vars->fd_out != 1) close(vars->fd_out);
		exit(127);
	}
	if (execve(path, vars->args, environment) < 0)
		(printf_global_error(127, 2, "my(s)hell: execve\n", vars->args[0]), exit(127));
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
		printf_global_error(128, 2, "fork\n");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (handle_child_cmd(vars, env, environment))
			return (g_last_exit_status);
	}
	/*else
	{
		waitpid(pid, &status, 0);
		g_last_exit_status = WEXITSTATUS(status);
	}*/
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_last_exit_status = WEXITSTATUS(status);
		else
			g_last_exit_status = 128;
	}
	return (0);
}