#include "lexer.h"

int	get_path(char *cmd, t_env *env , char **exec)
{
	int		i;
	char	**path;

	i = -1;
	path = ft_split(get_env_var(env, "PATH"), ':');
	if (path == NULL)
		return(error_message("Malloc error in split function"));
	while (path[++i])
	{
		//free(*exec);
		if (get_exec(path, i, cmd, exec))
			return (free_array(path), 1);
		if (access(exec, F_OK | X_OK) == 0)
			return (0);
		free(*exec);
	}
	free_array(path);
	*exec = cmd;
	return (0);
}

int	get_exec(char **path, int i, char *cmd, char **exec)
{
	char	*path_part;

	path_part = ft_strjoin(path[i], "/");
	if (path_part == NULL)
		return (error_message("Malloc error in strjoin function"));
	*exec = ft_strjoin(path_part, cmd);
	if (*exec == NULL)
	{
		free(path_part);
		return (error_message("Malloc error in strjoin function"));
	}
	free(path_part);
	return (0);
}

int	error_message(char *str) // Probably unnecessary
{
	perror(str);
	return (1);
}

void	free_array(char **array) //We probably have this one already
{
	size_t	i;

	i = -1;
	while (array[++i])
		free(array[i]);
	free(array);
}

void	execve_error(char **s_cmd) //Lets see if we wnt to use it
{
	perror("Execve failed");
	ft_putendl_fd("This command was not found: ", 2);
	ft_putendl_fd(s_cmd[0], 2);
	free_array(s_cmd);
	exit(0);
}