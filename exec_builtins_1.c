#include "minishell.h"

int	exec_builtins(t_exec_vars *vars, t_free_data *exec_data)
{
	if (ft_strcmp(vars->args[0], "exit") == 0)
		return (exec_exit(vars, exec_data));
	else if (ft_strcmp(vars->args[0], "cd") == 0)
		return (exec_cd(vars->args, &exec_data->env));
	else if (ft_strcmp(vars->args[0], "pwd") == 0)
		return (exec_pwd());
	else if (ft_strcmp(vars->args[0], "echo") == 0)
		return (exec_echo(vars));
	else if (ft_strcmp(vars->args[0], "export") == 0)
		return (exec_export(vars->args, &exec_data->env));
	else if (ft_strcmp(vars->args[0], "unset") == 0)
		return (exec_unset(vars->args, &exec_data->env));
	else if (ft_strcmp(vars->args[0], "env") == 0)
		return (exec_env(vars->args, exec_data->environment));
	return (3);
}

/* int	exec_global_env(t_exec_vars *vars, t_env **env, int *k)
{
	char	buffer[1024] = "";
	handle_dollar_sign(vars->args, buffer, env, k);

	vars->args[0][0] = *ft_strdup(buffer);
	return (0);
} */

int	ft_atoi_no_minus(const char *nptr)
{
	long int	number;

	number = 0;
	while (*nptr && (*nptr == 32 || (*nptr >= 9 && *nptr <= 13)))
	{
		nptr++;
	}
	if (*nptr == 43)
		nptr++;
	if ((*nptr <= 47) || (*nptr >= 58 && *nptr < 127))
		return (0);
	while (*nptr >= 48 && *nptr <= 57)
	{
		number = number * 10 + (*nptr - 48);
		nptr++;
	}
	return (number);
}

int	overflow_check(char *result)
{
	if (*result == '+')
		result++;
	while (*result == '0')
		result++;
	if (ft_atoi(result) == INT_MAX && (ft_strcmp(result, "2147483647") != 0
	|| ft_strcmp(result, "9223372036854775807") != 0))
		return (1);
	return (0);
}

int	exec_exit(t_exec_vars *vars, t_free_data *exec_data)
{
	int	i;
	char *result;

	i = 0;
	result = NULL;
	if (vars->args[1] != NULL)
	{
		if (vars->args[2] != NULL)
		{
			printf_global_error(1, 2, "my(s)hell: too many arguments\n");// Actually in bush +exit should be printed
			return(g_last_exit_status);
		}
		/* if (vars->args[1][i])
			return (printf_global_error(1, 2, "my(s)hell: too many arguments\n"),1);// Actually in bush +exit should be printed
		/*if (vars->args[1][i])
		{
			result = handle_quotes_echo(&vars->args[1][i],  &(vars->error));
			if (vars->error)
			{
				free(result);
				g_last_exit_status = vars->error;
				return(g_last_exit_status);
			}
		} */
		result = vars->args[1];
			if (result[i] == '+' || result[i] == '-')
				i++;
			while (result[i] == '0')
				i++;
			if (result[i] == '\0')
			{
				g_last_exit_status = 0;
				free_exit_data(exec_data);
				free_env_array(vars->args);
				free(vars);
				exit (g_last_exit_status);
			}
			i = 0;
			if (overflow_check(result))
			{
				printf_global_error(2, 2, "my(s)hell: exit: %s: numeric argument required\n", vars->args[1]);
				return(g_last_exit_status);
			}
		g_last_exit_status = ft_atoi(result);
	}
	if (vars->args[1] != NULL && !g_last_exit_status)
			{
				//free(result);
				printf_global_error(2, 2, "my(s)hell: exit: %s: numeric argument required\n", vars->args[1]);
				return(g_last_exit_status);
			}

	//if (result)
		//free(result);
	//printf("result: %s\n", result);
	//printf("result: %s\n", vars->args[1]);
	{
		free(result);
		printf_global_error(2, 2, "my(s)hell: exit: %s: numeric argument required\n", vars->args[1]);
		return(g_last_exit_status);
	}
	if (result)
		free(result);
	free_exit_data(exec_data);
	free_env_array(vars->args);
	free(vars);
	exit(g_last_exit_status);
}

int	exec_echo(t_exec_vars *vars)
{
	int	i;

	i = 1;
	//if (vars->args[i] == NULL || vars->args[i][0] == '\0')
		//return (printf_global_error(0, 1, "\n"), 0);
	if (ft_strcmp(vars->args[1], "-n") == 0)
		i++;
	//process_args(vars->args, &(vars->error));
	//if (vars->error)
		//return (free_env_array(vars->args), g_last_exit_status);
	while (vars->args[i])
	{
	        printf("%s", vars->args[i]);
	        if (vars->args[i + 1])
	            printf(" ");
	        i++;
	}
	if (ft_strcmp(vars->args[1], "-n") != 0)
		printf("\n");
	g_last_exit_status = 0;
	return (g_last_exit_status);
}

void	process_args(char **args, int *error)
{
	int i;
	char *new_str;

	i = 1;
	while (args[i])
	{
		new_str = handle_quotes_echo((args[i]), error);
		if (*error)
		{
			free(new_str);
			return ;
		}
		if (args[i])
			free(args[i]);
        args[i] = new_str;
        i++;
	}
}

char *handle_quotes_echo(const char *input, int *error)
{
	char *result;
	int i;
	int j;
	char quote;

    result = malloc(ft_strlen(input) + 1);
    if (!result)
	{
		*error = 1;
		return (printf_global_error(1, 2, "echo: memory allocation\n"), NULL);
	}
    i = 0;
	j = 0;
    while (input[i] != '\0')
	{
        if (input[i] == '"' || input[i] == '\'')
		{
            quote = input[i++];
            while (input[i] != '\0' && input[i] != quote)
                result[j++] = input[i++];
            if (input[i] != quote)
			{
                *error = 1;
				//return (printf_global_error(1, 2, "echo: no closing quote\n"), free(result), NULL);
				g_last_exit_status = 1;
				return (NULL);
			}
			i++;
		}
        else if (input[i] == '$' && input[i + 1] == '?')
		{

			result[j] = ft_itoa(g_last_exit_status)[0];
			i += 2;
			j = j + ft_strlen(&ft_itoa(g_last_exit_status)[0]);
		}
		else
            result[j++] = input[i++];
	}
    result[j] = '\0';
    return result;
}

int	exec_cd(char **args, t_env **env)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (printf_global_error(1, 2, "getcwd\n"), g_last_exit_status);
	if (args[1] != NULL && args[2])
		return (printf_global_error(1, 2, "cd: too many arguments\n"), free(cwd), g_last_exit_status);
	else if (args[1] == NULL || ft_strcmp(args[1], "~") == 0)
		return (change_directory_and_update(get_env_var(*env, "HOME"),
				env, cwd));
	else if (ft_strcmp(args[1], "..") == 0)
		return (change_directory_and_update("..", env, cwd));
	else
		return (change_directory_and_update(args[1], env, cwd));
}

int	exec_dollar_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (printf_global_error(1, 2, "getcwd\n"), g_last_exit_status);
	else
	{
		ft_putstr_fd(cwd, 1);
		ft_putstr_fd(": Is a directory\n", 1);
		g_last_exit_status = 126;
		free(cwd);
		return (g_last_exit_status);
	}
}

int	exec_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (printf_global_error(1, 2, "getcwd\n"), g_last_exit_status);
	else
	{
		printf("%s\n", cwd);
		free(cwd);
		g_last_exit_status = 0;
		return (g_last_exit_status);
	}
}

int	exec_export(char **args, t_env **env)
{
	char	*name;
	char	*value;
	int		control;

	name = NULL;
	value = NULL;
	if (args[1] == NULL)
		return (exec_export_no_args(*env), 0);
	control = var_control(args[1]);
	if (control == 1)
		return (g_last_exit_status);
	else
	{
		if (split_var(args[1], &name, &value))
			return (g_last_exit_status);
		if (update_add_env_var(env, name, value))
			return (free(name), free(value), g_last_exit_status);
	}
	g_last_exit_status = 0;
	return (g_last_exit_status);
}
