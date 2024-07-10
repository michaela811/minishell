/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:30:37 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 10:30:39 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	env_array = malloc((count + 1) * sizeof(char *));
	if (env_array == NULL)
		return (NULL);
	current = head;
	i = count - 1;
	while (current != NULL)
	{
		env_array[i] = create_env_str(current);
		if (env_array[i] == NULL)
		{
			free_env_array(env_array);
			return (NULL);
		}
		current = current->next;
		i--;
	}
	env_array[count] = NULL;
	return (env_array);
}

int get_cwd(char *cmd, char **exec, char **path)
{
    char *cwd;

	cwd = getcwd(NULL, 0); // Dynamically allocate buffer for current directory
    if (!cwd)
		return (print_err(1, 2, "malloc error in getcwd function\n"), 1);
	path[0] = cwd;
	if (get_exec(path, 0, cmd, exec))
			return (free(cwd),free_array(path), g_last_exit_status);
    if (access(*exec, F_OK | X_OK) == 0)
	{
       // *exec = path; // Set exec to the full path
        free(cwd);
        return (0); // Success
    }
    free(cwd);
	cwd = NULL;
	*exec = cmd;
    return (-1);
}

int count_double_colon(char *pre_path)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while(pre_path[i])
	{
		if(pre_path[i] == ':' && pre_path[i + 1] == ':')
			count++;
		i++;
	}
	//printf("additional: %d\n", count);
	return (count);
}

int detect_double_colons(char **pre_path)
{
	char *original;
	char *new_path;
	int i;
	int j;

    i = 0;
	j = 0;
	original = *pre_path;
    new_path = malloc(ft_strlen(original) + count_double_colon(original) + 1);
	if (!new_path)
		return (1);
	while (original[i])
	{
        if (original[i] == ':' && original[i + 1] == ':')
		{
            new_path[j++] = ':';
            new_path[j++] = '.';
            i++;
        }
		else
            new_path[j++] = original[i++];
    }
    new_path[j] = '\0';
    free(*pre_path);
    *pre_path = new_path;
    return (0);
}

int handle_colon (char **pre_path, t_env *env)
{
	int len;
	char *tmp_path;

	*pre_path = ft_strdup(get_env_var(env, "PATH"));
	if (!*pre_path)
		return (1);
	if ((*pre_path)[0] == ':')
	{
		tmp_path = ft_strjoin (".", *pre_path);
		if (!tmp_path)
			return (free(*pre_path), 1);
		free (*pre_path);
		*pre_path = tmp_path;
	}
	len = ft_strlen(*pre_path);
	if ((*pre_path)[len - 1] == ':')
	{
		tmp_path = ft_strjoin (*pre_path, ".");
		if (!tmp_path)
			return (free(*pre_path), 1);
		free (*pre_path);
		*pre_path = tmp_path;
	}
	if (detect_double_colons(pre_path))
		return (free(*pre_path), 1);
	return(0);
}

int	get_path(char *cmd, t_env *env, char **exec)
{
	int		i;
	char	**path;
	char	*pre_path;

	i = -1;
	if (handle_colon(&pre_path, env))
		return (print_err(1, 2,
				"malloc error in handle_colon function\n"), 1);
	path = ft_split(get_env_var(env, "PATH"), ':');
	if (path == NULL)
		return (free(pre_path), print_err(1, 2,
				"malloc error in split function\n"), 1);
	free (pre_path);
	if (path[0] == NULL)
	{
		if(get_cwd(cmd, exec, path))
			return(free(path), -1);
		return (free_array(path), 0);
	}
	while (path[++i])
	{
		if (get_exec(path, i, cmd, exec))
			return (free_array(path), g_last_exit_status);
		if (access(*exec, F_OK | X_OK) == 0)
			return (free_array(path), 0);
		if (!path[i + 1] && strcmp(path[i], getenv("HOME")) == 0)
			return (-2);
		//if (path[i + 1])
		free(*exec);
	}
	free_array(path);
	*exec = cmd;
	return (-1);
}

int	get_exec(char **path, int i, char *cmd, char **exec)
{
	char	*path_part;

	path_part = ft_strjoin(path[i], "/");
	if (path_part == NULL)
		return (print_err(1, 2,
				"malloc error in strjoin function\n"), 1);
	*exec = ft_strjoin(path_part, cmd);
	if (*exec == NULL)
	{
		free(path_part);
		return (print_err(1, 2,
				"malloc error in strjoin function\n"), 1);
	}
	free(path_part);
	return (0);
}
