#include "minishell.h"
#include <errno.h>

int	execute_command(t_exec_vars *vars, t_free_data *free_data)
{
	//char	**environment;
	int		return_builtins;

	if (!vars->args[0])
		return (0);
	free_data->environment = env_list_to_array(free_data->env);
	if (free_data->environment == NULL)
	{
		g_last_exit_status = 1;
		return (free_command_data(free_data), 1);
		return (g_last_exit_status);
	}
	return_builtins = exec_builtins(vars, free_data);
	if (return_builtins == 2)
	{
		if (handle_fork(vars, &free_data->env, free_data->environment))
		{
			//g_last_exit_status = 1;
			return (free_command_data(free_data), g_last_exit_status);
		}
	}
	else if (return_builtins == 1)
	{
		//g_last_exit_status = 1;
		return (free_command_data(free_data), g_last_exit_status);
	}
	return (free_command_data(free_data), g_last_exit_status);
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

	process_args(vars->args, &vars->error);
	if (vars->error)
		return (g_last_exit_status);
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
	path_status = get_path(vars->args[0], *env, &path);
	if (path_status == 1)
		return (g_last_exit_status);
	if (path_status == -1)
	{
    	struct stat path_stat;
    	stat(path, &path_stat);
    	if (access(path, F_OK) == -1 || S_ISDIR(path_stat.st_mode))
			return (printf_global_error(127, 2, "my(s)hell: %s: command not found\n", vars->args[0]), 127);
    	if (access(vars->args[0], X_OK) == -1)
		{
        if (S_ISREG(path_stat.st_mode))
			{
        	    ft_putstr_fd("my(s)hell: ", 2);
        	    ft_putstr_fd(vars->args[0], 2);
				if ((path_stat.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) == 0) {
                // The file has execute bits set but isn't executable, perhaps due to other restrictions
				//ft_putstr_fd(": command not found\n", 2);
        	        //return (g_last_exit_status = 127);
                ft_putstr_fd(": Permission denied\n", 2);
        	        return (g_last_exit_status = 126);
            } else {
                // The file has no execute bits set
                ft_putstr_fd(": Permission denied\n", 2);
        	        return (g_last_exit_status = 126);
            }
        	    /*if ((path_stat.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) != 0)
				{
        	        ft_putstr_fd(": command not found\n", 2);
        	        return (g_last_exit_status = 127);
        	    } else {
        	        ft_putstr_fd(": Permission denied\n", 2);
        	        return (g_last_exit_status = 126);
        	    }*/
        	}
		}
	}
//free(path); //probably freed later as lexem
	if (execve(path, vars->args, environment) < 0)
		return (printf_global_error(127, 2, "my(s)hell: execve\n", vars->args[0]), 127);
	g_last_exit_status = 0;
	exit(EXIT_SUCCESS);
	//return (g_last_exit_status);
}

int	handle_fork(t_exec_vars *vars, t_env **env, char **environment)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		free_env_array(environment);
		printf_global_error(128, 2, "fork\n");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (handle_child_cmd(vars, env, environment))
			return (g_last_exit_status);
	}
	else
	{
		waitpid(pid, &status, 0);
		g_last_exit_status = WEXITSTATUS(status);
	}
	free_env_array(environment);
	return (0);
}
