#include "lexer.h"

char	*get_path(char *cmd, char **env)
{
	int		i;
	char	*exec;
	char	**path;

	i = -1;
	path = ft_split(getenv("PATH"), ':');
	if (path == NULL)
		error_message("Malloc error in split function");
	while (path[++i])
	{
		exec = get_exec(path, i, cmd);
		if (access(exec, F_OK | X_OK) == 0)
			return (exec);
		free(exec);
	}
	free_array(path);
	return (cmd);
}

char	*get_exec(char **path, int i, char *cmd)
{
	char	*exec;
	char	*path_part;

	path_part = ft_strjoin(path[i], "/");
	if (path_part == NULL)
		error_message("Malloc error in strjoin function");
	exec = ft_strjoin(path_part, cmd);
	if (exec == NULL)
		error_message("Malloc error in strjoin function");
	free(path_part);
	return (exec);
}

void	error_message(char *str) // Probably unnecessary
{
	perror(str);
	exit (1);
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