#include "minishell.h"
#include <errno.h>

int	execute_command(t_exec_vars *vars, t_env **env)
{
	char	**environment;
	int		return_builtins;

	if (!vars->args[0])
		return (0);
	environment = env_list_to_array(*env);
	if (environment == NULL)
	{
		g_last_exit_status = 1;
		return (g_last_exit_status);
	}
	return_builtins = exec_builtins(vars, env, environment);
	if (return_builtins == 2)
	{
		if (handle_fork(vars, env, environment))
		{
			//g_last_exit_status = 1;
			return (free_env_array(environment), g_last_exit_status);
		}
	}
	else if (return_builtins == 1)
	{
		g_last_exit_status = 1;
		return (free_env_array(environment), g_last_exit_status);
	}
	return (free_env_array(environment), g_last_exit_status);
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

int	handle_child_cmd(t_exec_vars *vars, t_env **env, char **environment)
{
	char	*path;
	int		path_status;

	//if (ft_strcmp(vars->args[0], "") == 0)
		//return(0);
	process_args(vars->args, &(vars->error));
	if (vars->error)
		return (perror("execve preprocessing: odd number of quotes\n"), 1);
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
	    {
	        ft_putstr_fd(vars->args[0], 2);
	        ft_putstr_fd(": No such file or directory\n", 2);
	        g_last_exit_status = 127;
	        return(g_last_exit_status);
	    }
	    else if (dir_check)
	    {
	        ft_putstr_fd(vars->args[0], 2);
	        ft_putstr_fd(": Is a directory\n", 2);
	        g_last_exit_status = 126;
	        return(g_last_exit_status);
	    }
	}
	path_status = get_path(vars->args[0], *env, &path);
	if (path_status == 1)
	{
		g_last_exit_status = 138;
		return (g_last_exit_status);
	}
	if (path_status == -1)
{
    struct stat path_stat;
    stat(path, &path_stat);
    if (access(path, F_OK) == -1 || S_ISDIR(path_stat.st_mode))
    {
        ft_putstr_fd("my(s)hell: ", 2);
        ft_putstr_fd(vars->args[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        g_last_exit_status = 127;
        return (g_last_exit_status);
    }
    else if (access(path, X_OK) == -1 && S_ISREG(path_stat.st_mode))
    {
        ft_putstr_fd("my(s)hell: ", 2);
        ft_putstr_fd(vars->args[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        g_last_exit_status = 127;
        return (g_last_exit_status);
    }
    else
    {
        ft_putstr_fd("my(s)hell: ", 2);
        ft_putstr_fd(vars->args[0], 2);
        ft_putstr_fd(": Permission denied\n", 2);
        g_last_exit_status = 126;
        return (g_last_exit_status);
    }
}
//free(path); //probably freed later as lexem
	if (execve(path, vars->args, environment) < 0)
	{
		g_last_exit_status = 127;
		perror("execve");
		//exit(EXIT_FAILURE);
		return(1);
	}
	//g_last_exit_status = 0;
	//exit(EXIT_SUCCESS);
	return (g_last_exit_status);
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
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		if (handle_child_cmd(vars, env, environment))
		{
			//g_last_exit_status = 155;
			return (g_last_exit_status);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		g_last_exit_status = WEXITSTATUS(status);
	}
	return (0);
}
