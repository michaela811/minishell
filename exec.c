#include "lexer.h"

void	execute(char *cmd, char **env)
{
	char	**s_cmd;
	char	*path;

	s_cmd = ft_split(cmd, ' ');
	if (s_cmd == NULL)
		error_message("Malloc error in split function: ");
	path = get_path(s_cmd[0], env);
	if (execve(path, s_cmd, env) == -1)
		execve_error(s_cmd);
}

char	*get_path(char *cmd, char **env)
{
	int		i;
	char	*exec;
	char	**path;
	char	**s_cmd;

	i = -1;
	path = ft_split(get_env(env, "PATH"), ':');
	if (path == NULL)
		error_message("Malloc error in split function");
	s_cmd = ft_split(cmd, ' ');
	if (s_cmd == NULL)
		error_message("Malloc error in split function");
	while (path[++i])
	{
		exec = get_exec(path, i, s_cmd);
		if (access(exec, F_OK | X_OK) == 0)
		{
			free_array(s_cmd);
			return (exec);
		}
		free(exec);
	}
	free_array(path);
	free_array(s_cmd);
	return (cmd);
}

char	*get_env(char **env, char *var)
{
	int		i;
	int		j;
	char	*s_var;

	i = -1;
	while (env[++i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		s_var = ft_substr(env[i], 0, j);
		if (s_var == NULL)
			error_message("Malloc error in substr function");
		if (ft_strcmp(s_var, var) == 0)
		{
			free(s_var);
			return (env[i] + j + 1);
		}
		free(s_var);
	}
	return (NULL);
}

char	*get_exec(char **path, int i, char **s_cmd)
{
	char	*exec;
	char	*path_part;

	path_part = ft_strjoin(path[i], "/");
	if (path_part == NULL)
		error_message("Malloc error in strjoin function");
	exec = ft_strjoin(path_part, s_cmd[0]);
	if (exec == NULL)
		error_message("Malloc error in strjoin function");
	free(path_part);
	return (exec);
}

void	error_message(char *str)
{
	perror(str);
	exit (1);
}

void	free_array(char **array)
{
	size_t	i;

	i = -1;
	while (array[++i])
		free(array[i]);
	free(array);
}

void	exit_function(int i)
{
	if (i == 1)
		ft_putstr_fd("Please enter correct input:\n./pipex"
			" infile \"cmd\" \"cmd\" outfile\n", 2);
	else if (i == 2)
		ft_putstr_fd("The infile does not exists.\n", 2);
	exit(0);
}

void	execve_error(char **s_cmd)
{
	perror("Execve failed");
	ft_putendl_fd("This command was not found: ", 2);
	ft_putendl_fd(s_cmd[0], 2);
	free_array(s_cmd);
	exit(0);
}