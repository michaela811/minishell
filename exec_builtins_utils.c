#include "minishell.h"

/*void normalize_path(char **path) {
    if (path == NULL || *path == NULL) return;

    char *input = *path;
    int len = strlen(input);
    char *normalized = calloc(len + 1, sizeof(char));  // Use calloc to initialize to zero
    if (normalized == NULL) return;

    char **components = malloc(sizeof(char*) * (len + 1)); // For splitting the path
    if (components == NULL) {
        free(normalized);
        return;
    }
    int count = 0;

    const char *delim = "/";
    char *token = strtok(input, delim);

    while (token != NULL) {
        if (strcmp(token, "..") == 0) {
            // Go up in the directory tree
            if (count > 0) count--;
        } else if (strcmp(token, ".") != 0) {
            // Ignore '.' which stands for current directory
            components[count++] = token;
        }
        token = strtok(NULL, delim);
    }

    if (input[0] == '/') {
        normalized[0] = '/';
    }

    for (int i = 0; i < count; i++) {
        if (i > 0 || input[0] == '/') strcat(normalized, "/"); // Only add '/' if not the first component or it's an absolute path
        strcat(normalized, components[i]);
    }

    free(components);
    *path = normalized;  // Update the path with the normalized result
}*/

void normalize_path(char **path) {
    if (path == NULL || *path == NULL) return;

    const char *input = *path;
    int len = strlen(input);
    char *normalized = calloc(len + 1, sizeof(char));  // Use calloc to initialize to zero
    if (normalized == NULL) return;

    char **components = malloc(sizeof(char*) * (len + 1)); // For splitting the path
    if (components == NULL) {
        free(normalized);
        return;
    }
    int count = 0;
    char *temp = strdup(input);  // Create a modifiable copy of input
    if (temp == NULL) {
        free(components);
        free(normalized);
        return;
    }

    const char *delim = "/";
    char *token, *saveptr;
    token = strtok_r(temp, delim, &saveptr);

    while (token != NULL) {
        if (strcmp(token, "..") == 0) {
            if (count > 0) count--;  // Go up in the directory tree, ignore if at root
        } else if (strcmp(token, ".") != 0 && strlen(token) > 0) {
            components[count++] = token;  // Ignore '.' and empty tokens
        }
        token = strtok_r(NULL, delim, &saveptr);
    }

    if (input[0] == '/') {
        normalized[0] = '/';
    }

    for (int i = 0; i < count; i++) {
        strcat(normalized, components[i]);
        if (i < count - 1 || input[len-1] == '/') strcat(normalized, "/"); // Add '/' if not the last component or path ends with '/'
    }

    free(temp);
    free(components);
    *path = normalized;  // Update the path with the normalized result
}

bool needs_normalization(const char *path)
{
    return strstr(path, "//") != NULL || strstr(path, "/./") != NULL || strstr(path, "/../") != NULL;
}

int	change_directory_and_update(char *path, t_env **env, char *cwd, char **args)//, int line)
{
    if (needs_normalization(path))
    {
        normalize_path(&path);
    }
	if (chdir(path) != 0)
	{
		printf_global_error(1, 2, "my(s)hell: %s: %s: No such file or directory\n", args[0], path);
		/*g_last_exit_status = 1;
		perror("chdir");*/
		free(cwd);
		return (1);
	}
	if (update_pwd(env, cwd))
	{
		g_last_exit_status = 1;
		free(cwd);
		return (1);
	}
	free(cwd);
	g_last_exit_status = 0;
	return (g_last_exit_status);
}

void	exec_export_no_args(t_env *env)
{
	t_env	*current;

	current = env;
	while (current != NULL)
	{
		//printf("declare -x %s=\"%s\"\n", current->name, current->value);
		printf("export %s=\"%s\"\n", current->name, current->value);
		current = current->next;
	}
}

int	var_control(char *command, char *args)
{
	int	i;

	i = 0;
	if (args[i] == '=' || args[i] == 0 || (!ft_isalpha(args[i]) && args[i] != '_'))
	{
		return (printf_global_error(1, 2, "mini(s)hell: %s: '%s': not a valid identifier\n", command, args), g_last_exit_status);
	}
	i++;
	while (args[i] && args[i] != '=')
    {
        if (!ft_isalnum(args[i]) && args[i] != '_')
        {
            return (printf_global_error(1, 2, "mini(s)hell: %s: '%s': not a valid identifier\n", command, args), g_last_exit_status);
        }
        i++;
    }
	return (0);
}

int	split_var(char *var, char **name, char **value)
{
	char	*equals;
	int		i;

	i = 1;
	equals = ft_strchr(var, '=');
	if (equals)
		*name = ft_strndup(var, equals - var);
	else
		*name = ft_strdup(var);
	if (name == NULL)
		return (printf_global_error(1, 2, "split_var: strndup error\n"), free(*name), g_last_exit_status);
	while ((*name)[i] != '\0')
	{
		if (!ft_isalnum((*name)[i]) && (*name)[i] != '_')
			return(printf_global_error(1, 2, "mini(s)hell: %s: %s: not a valid identifier\n", var[0], *name), free(*name), g_last_exit_status);
		i++;
	}
	(*name)[i] = '\0';
	if (equals == NULL)
	{
		*value = ft_strdup("");
		if (value == NULL)
			return (printf_global_error(1, 2, "split_var: strndup error\n"), free(*name), g_last_exit_status);
		return (0);
	}
	*value = ft_strdup(equals + 1);//MEMORY!!!
	//if (export_quotes(equals + 1, value))
		//return (printf_global_error(1, 2, "split_var: export_quotes error\n"), free(*name), g_last_exit_status);
	//if (value == NULL)//Do we need this check?
		//return (printf_global_error("split_var: strndup error\n"), free(*name), 1);
	return (0);
}

int export_quotes(char *input, char **output)
{
    int len;
	len = ft_strlen(input);
    if (len == 0)
	{
        *output = ft_strdup(""); // Handle empty value
        return (0);
    }
	if ((input[0] == '"' || input[0] == '\'') && input[0] != input[len - 1])
	{
		g_last_exit_status = 1;//maybe to change it to a different number
		return(ft_printf_fd(1, "export_quotes: unbalanced quotes"), g_last_exit_status);
	}
    if ((input[0] == '"' || input[0] == '\'') && input[0] == input[len - 1])
	{
        *output = ft_strndup(input + 1, len - 2);
        if (*output == NULL)
		{
			g_last_exit_status = 1;//maybe to change it to a different number
			return(ft_printf_fd(1, "export_quotes: strndup error"), g_last_exit_status);
        }
    }
	else
	{
        *output = ft_strdup(input); // No quotes to handle, just duplicate
        if (*output == NULL)
		{
            g_last_exit_status = 1;//maybe to change it to a different number
			return(ft_printf_fd(1, "export_quotes: strndup error"), g_last_exit_status);
        }
    }
    return 0;
}

int	update_pwd(t_env **env, char *cwd)
{
	if (update_add_env_var(env, "OLDPWD", cwd))
		return (1);
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (perror("getcwd"), 1);
	if (update_add_env_var(env, "PWD", cwd))
		return (free(cwd), 1);
	return (free(cwd), 0);
}
