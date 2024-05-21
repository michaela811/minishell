#include "minishell.h"

t_env	*init_environment(char **envt)
{
	t_env	*head;
	char	**env;
	char	*entry;
	char	*separator;
	t_env	*var;

	head = NULL;
	env = envt;
	while (*env)
	{
		entry = *env;
		separator = ft_strchr(entry, '=');
		if (separator)
		{
			*separator = '\0';
			var = create_env_var(entry, separator + 1);
			if (var == NULL)
				return (free_env(head), NULL);
			*separator = '=';
			var->next = head;
			head = var;
		}
		env++;
	}
	return (head);
}

//TO DISCUSS ABOUT CONST
int	update_add_env_var(t_env **head, const char *name, const char *value)
{
	t_env	*var;
	t_env	*new_var;

	var = find_env_var(*head, name);
	if (var)
	{
		free(var->value);
		var->value = ft_strdup(value);
		if (var->value == NULL)
			return (perror("Memory allocation error"), 1);
	}
	else
	{
		new_var = create_env_var(name, value);
		if (new_var == NULL)
			return (perror("Memory allocation error"), 1);
		new_var->next = *head;
		*head = new_var;
	}
	return (0);
}

char	**env_list_to_array(t_env *head)
{
	int		count;
	char	**env_array;
	t_env	*current;
	int		i;

	count = count_env_list(head);
	env_array = MY_MALLOC((count + 1) * sizeof(char *));
	if (env_array == NULL)
		return (NULL);
	current = head;
	i = 0;
	while (i < count)
	{
		env_array[i] = create_env_str(current);
		if (env_array[i] == NULL)
		{
			free_env_array(env_array);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	env_array[count] = NULL;
	return (env_array);
}

int	get_path(char *cmd, t_env *env, char **exec)
{
	int		i;
	char	**path;

	i = -1;
	path = ft_split(get_env_var(env, "PATH"), ':');
	if (path == NULL)
		return (printf_global_error(1, 2, "MY_MALLOC error in split function\n"), 1);
	while (path[++i])
	{
		if (get_exec(path, i, cmd, exec))
			return (free_array(path), g_last_exit_status);
		if (access(*exec, F_OK | X_OK) == 0)
			return (free_array(path), 0);
		free(*exec);
	}
	free_array(path);//maybe will be freed later as lexem!
	*exec = cmd;
	return (-1);
}

int	get_exec(char **path, int i, char *cmd, char **exec)
{
	char	*path_part;

	path_part = ft_strjoin(path[i], "/");
	if (path_part == NULL)
		return (printf_global_error(1, 2, "MY_MALLOC error in strjoin function\n"), 1);
	*exec = ft_strjoin(path_part, cmd);
	if (*exec == NULL)
	{
		free(path_part);
		return (printf_global_error(1, 2, "MY_MALLOC error in strjoin function\n"), 1);
	}
	free(path_part);
	return (0);
}
