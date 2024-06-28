#include <minishell.h>

int	handle_quotes_in_echo(char *input, char *result, int *i, int *j)
{
	char	quote;

	if (input[*i] == '"' || input[*i] == '\'')
	{
		quote = input[(*i)++];
		while (input[*i] != '\0' && input[*i] != quote)
			result[(*j)++] = input[(*i)++];
		if (input[*i] != quote)
		{
			g_last_exit_status = 1;
			return (1);
		}
		(*i)++;
	}
	return (0);
}

int	malloc_result(char **result, char *input, int *error)
{
	*result = malloc(ft_strlen(input) + 1);
	if (!(*result))
	{
		*error = 1;
		return (1);
	}
	return (0);
}

char	*handle_quotes_echo(char *input, int *error)
{
	char	*result;
	int		i;
	int		j;

	if (malloc_result(&result, input, error))
		return (print_err(1, 2, "echo: memory allocation\n"), NULL);
	i = 0;
	j = 0;
	while (input[i] != '\0')
	{
		if (handle_quotes_in_echo(input, result, &i, &j))
		{
			*error = 1;
			return (free(result), NULL);
		}
		else if (input[i] == '$' && input[i + 1] == '?')
			return_exit_status(result, &i, &j, error);
		if (*error)
			return (free(result), NULL);
		else
			result[j++] = input[i++];
	}
	result[j] = '\0';
	return (result);
}

int	exec_cd(char **args, t_env **env)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (print_err(1, 2, "getcwd\n"), g_last_exit_status);
	if (args[1] != NULL && args[2])
		return (print_err(1, 2, "my(s)hell: %s: too many arguments\n",
				args[0]), free(cwd), g_last_exit_status);
	else if (args[1] == NULL || ft_strcmp(args[1], "~") == 0)
		return (change_directory_and_update(get_env_var(*env, "HOME"),
				env, cwd, args));
	else if (ft_strcmp(args[1], "..") == 0)
		return (change_directory_and_update("..", env, cwd, args));
	else if (ft_strcmp(args[1], "-") == 0)
	{
		if (get_env_var(*env, "OLDPWD") == NULL)
			return (print_err(1, 2, "my(s)hell: %s: OLDPWD not set\n",
					args[0]), g_last_exit_status);
		change_directory_and_update(get_env_var(*env, "OLDPWD"),
			env, cwd, args);
		printf("%s\n", get_env_var(*env, "PWD"));
		return (g_last_exit_status);
	}
	else
		return (change_directory_and_update(args[1], env, cwd, args));
}

int	exec_dollar_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (print_err(1, 2, "getcwd\n"), g_last_exit_status);
	else
	{
		ft_putstr_fd(cwd, 1);
		ft_putstr_fd(": Is a directory\n", 1);
		g_last_exit_status = 1;
		free(cwd);
		return (g_last_exit_status);
	}
}


