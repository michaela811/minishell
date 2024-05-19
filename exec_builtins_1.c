#include "minishell.h"

int	exec_builtins(t_exec_vars *vars, t_free_data *free_data)
{
	if (ft_strcmp(vars->args[0], "exit") == 0)
		return (exec_exit(vars, free_data));
	else if (ft_strcmp(vars->args[0], "cd") == 0)
		return (exec_cd(vars->args, &free_data->env));
	else if (ft_strcmp(vars->args[0], "pwd") == 0)
		return (exec_pwd());
	else if (ft_strcmp(vars->args[0], "echo") == 0)
		return (exec_echo(vars));
	else if (ft_strcmp(vars->args[0], "export") == 0)
		return (exec_export(vars->args, &free_data->env));
	else if (ft_strcmp(vars->args[0], "unset") == 0)
		return (exec_unset(vars->args, &free_data->env));
	else if (ft_strcmp(vars->args[0], "env") == 0)
		return (exec_env(vars->args, free_data->environment));
	return (2);
}

int	exec_global_env(t_exec_vars *vars, t_env **env)
{
	char	buffer[1024] = "";
	handle_dollar_sign(vars->args, buffer, env);

	vars->args[0][0] = *ft_strdup(buffer);
	return (0);
}

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

int	exec_exit(t_exec_vars *vars, t_free_data *free_data)
{
	int	i;
	char *result;

	i = 0;
	result = NULL;
	if (vars->args[1] != NULL)
	{
		if (vars->args[2] != NULL)
			return (printf_global_error(1, 2, "my(s)hell: too many arguments"),1);// Actually in bush +exit should be printed
		if (vars->args[1][i])
		{
			if (vars->args[1][0] == '+')
				i++;
			result = handle_quotes_echo(&vars->args[1][i],  &(vars->error));
			if (vars->error)
			{
				free(result);
				free_command_data(free_data);
				g_last_exit_status = vars->error;
				exit(g_last_exit_status);
			}
		}
		i = 0;
		while (result && result[i])
		{
			if (result[i] == '+' && vars->args[1][0] == '+')
			{
				free(result);
				free_command_data(free_data);
				g_last_exit_status = 156;
				exit(g_last_exit_status);
			}
			if (result[i] == '+')
				i++;
			if (ft_atoi(result) == 0)
			{
				g_last_exit_status = 2;
				exit(g_last_exit_status);
			}
			if (ft_isdigit(result[i]) == 0)
			{
				free(result);
				free_command_data(free_data);
				g_last_exit_status = 156;
				exit(g_last_exit_status);
			}
			i++;
		}
		g_last_exit_status = ft_atoi(result);
	}
	free(result);
	free_command_data(free_data);
	//check_for_memory_leaks();
	exit(g_last_exit_status);
}

int	exec_echo(t_exec_vars *vars)
{
	int	i;

	i = 1;
	if (vars->args[i] == NULL || vars->args[i][0] == '\0')
		return (printf_global_error(0, 1, "\n"), 0);
	if (ft_strcmp(vars->args[i], "$?") == 0)
	{
		printf("%d\n", g_last_exit_status);
		g_last_exit_status = 0;
		return (g_last_exit_status);
	}
	else if (ft_strcmp(vars->args[1], "-n") == 0)
		i++;
	process_args(vars->args, &(vars->error));
	if (vars->error) //free the args
		return (free_env_array(vars->args), g_last_exit_status);
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
		*error = 1;//Do we need this error or can be handle with g_last_exit_status
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
				return (printf_global_error(1, 2, "echo: no closing quote\n"), free(result), NULL);
			}
			i++;
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
