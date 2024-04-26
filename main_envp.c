#include "lexer.h"

t_env *create_env_var(const char *name, const char *value)
{
    t_env *var = malloc(sizeof(t_env));
    if (var == NULL)
		return NULL;
    var->name = ft_strdup(name);
	if (var->name == NULL)
		return(free(var), NULL);
    var->value = ft_strdup(value);
	if (var->value == NULL)
		return(free(var->name), free(var), NULL);
    var->next = NULL;
    return var;
}

t_env *init_environment(char **envp)
{
    t_env *head = NULL;
    char **env = envp;
    while (*env)
	{
        char *entry = *env;
        char *separator = ft_strchr(entry, '=');
        if (separator)
		{
            *separator = '\0';  // Temporarily terminate the string to isolate the name
            t_env *var = create_env_var(entry, separator + 1);
			if (var == NULL)
				return (free_env(head), NULL);
            *separator = '=';  // Restore the environment string
            var->next = head;
            head = var;
        }
        env++;
    }
    return head;
}

char *get_env_var(t_env *head, const char *name)
{
    while (head != NULL) {
        if (ft_strcmp(head->name, name) == 0) {
            return head->value;  // Return the value of the variable if found
        }
        head = head->next;  // Move to the next node in the list
    }
    return NULL;  // Return NULL if the variable is not found
}

t_env *find_env_var(t_env *head, const char *name)
{
    while (head)
	{
        if (ft_strcmp(head->name, name) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

int update_add_env_var(t_env **head, const char *name, const char *value)
{
    t_env *var;
    t_env *new_var;

    var = find_env_var(*head, name);
    if (var)
	{
        free(var->value);
        var->value = ft_strdup(value);
        if (var->value == NULL)
            return (perror("Memory allocation error"), 1);
    }
	else//Probably we don't need this as we don't implement
	{
        new_var = create_env_var(name, value);
        if (new_var == NULL)
            return (perror("Memory allocation error"), 1);
        new_var->next = *head;
        *head = new_var;
    }
    return (0);
}

int count_env_list(t_env *head)
{
    int count = 0;
    t_env *current = head;
    while (current != NULL)
    {
        count++;
        current = current->next;
    }
    return count;
}

char *create_env_str(t_env *current)
{
    char *env_str = malloc(ft_strlen(current->name) + ft_strlen(current->value) + 2);
    if (env_str == NULL)
        return NULL;
    ft_strcpy(env_str, current->name);
    ft_strcat(env_str, "=");
    ft_strcat(env_str, current->value);
    return env_str;
}

char **env_list_to_array(t_env *head)
{
    int count;
    char **env_array;

    count = count_env_list(head);
    env_array = malloc((count + 1) * sizeof(char *));
    if (env_array == NULL)
        return (NULL);
    t_env *current = head;
    for (int i = 0; i < count; i++)
    {
        env_array[i] = create_env_str(current);
        if (env_array[i] == NULL)
        {
            free_env_array(env_array);
            return (NULL);
        }
        current = current->next;
    }
    env_array[count] = NULL;
    return (env_array);
}

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
		{
			g_last_exit_status = 1;
			return (free_array(path), 1);
		}
		if (access(*exec, F_OK | X_OK) == 0)
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


