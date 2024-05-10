#include "minishell.h"

int	exec_builtins(t_exec_vars *vars, t_env **env, char **environment)
{
	if (ft_strcmp(vars->args[0], "exit") == 0)
		return (exec_exit(vars, env));
	else if (ft_strcmp(vars->args[0], "cd") == 0)
		return (exec_cd(vars->args, env));
	else if (ft_strcmp(vars->args[0], "pwd") == 0)
		return (exec_pwd());
	//else if (ft_strcmp(vars->args[0], "$PWD") == 0)
		//return (exec_dollar_pwd());
	else if (ft_strcmp(vars->args[0], "echo") == 0)
		return (exec_echo(vars));
	else if (ft_strcmp(vars->args[0], "export") == 0)
		return (exec_export(vars->args, env));
	else if (ft_strcmp(vars->args[0], "unset") == 0)
		return (exec_unset(vars->args, env));
	else if (ft_strcmp(vars->args[0], "env") == 0)
		return (exec_env(vars->args, environment));
	//else if (vars->args[0][0] == '$')
		//return (exec_global_env(vars, env));
	return (2);
}

int	exec_global_env(t_exec_vars *vars, t_env **env)
{
	//char	*str;
	char	buffer[1024] = "";
	//char	*start;
	handle_dollar_sign(vars->args, buffer, env);

	vars->args[0][0] = *ft_strdup(buffer);
	return (0);
}
/*
void	handle_dollar_sign(char **start, char *buffer, t_env **env)
{
	char	*dollar;
	char	*var_start;
	char	*var_end;
	char	var_name[1024];
	char	*var_value;
	char	*start_store;

	//buffer = NULL;
	start_store = *start;
	while ((dollar = ft_strchr(*start, '$')) != NULL)
	{
		ft_strncat(buffer, *start, dollar - *start);
		if (*(dollar + 1) == '?')
		{
			ft_strcat(buffer, ft_itoa(g_last_exit_status));
			*start = dollar + 2;
		}
		else if (*(dollar + 1) == '\0' || *(dollar + 1) == '$' || *(dollar + 1) == ' ' || *(dollar + 1) == '"')
		{
			ft_strcat(buffer, "$");
			*start = dollar + 1;
		}
		else
		{
			var_start = dollar + 1;
			var_end = ft_strpbrk(var_start, " \t\n\"'$");
			if (var_end == NULL)
				var_end = var_start + ft_strlen(var_start);
			ft_strncpy(var_name, var_start, var_end - var_start);
			var_name[var_end - var_start] = '\0';
			var_value = get_env_var(*env, var_name);
			if (var_value != NULL)
				ft_strcat(buffer, var_value);
			*start = var_end;
		}
	}
	if ((dollar = ft_strchr(start_store, '$')) == NULL)
		ft_strcpy(buffer, start_store);//Probably use strncpy!
	else if (*start)
		ft_strcat(buffer, *start);
}
}*/

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
	if ((*nptr <= 47) || (*nptr >= 58 && *nptr < 127))//removed *nptr >= 0 &&
		return (0);
	while (*nptr >= 48 && *nptr <= 57)
	{
		number = number * 10 + (*nptr - 48);
		nptr++;
	}
	return (number);
}

int	exec_exit(t_exec_vars *vars, t_env **env)
{
	int	i;
	char *result;

	i = 0;
	result = NULL;
	if (vars->args[1] != NULL)
	{
		if (vars->args[2] != NULL)
		{
			//printf("exit: too many arguments");
			g_last_exit_status = 1;
			exit(g_last_exit_status);
		}
		if (vars->args[1][i])
		{
			if (vars->args[1][0] == '+')
				i++;
			result = handle_quotes_echo(&vars->args[1][i], &(vars->error));
			if (vars->error)
			{
				g_last_exit_status = vars->error;
				exit(g_last_exit_status);
			}
		}
		i = 0;
		while (result && result[i])
		{
			if (result[i] == '+' && vars->args[1][0] == '+')
			{
				g_last_exit_status = 156;
				exit(g_last_exit_status);
			}
			if (result[i] == '+')
				i++;
			if (ft_isdigit(result[i]) == 0)
			{
				g_last_exit_status = 156;
				exit(g_last_exit_status);
			}
			i++;
		}
		g_last_exit_status = ft_atoi(result);
	}
	free_env(*env);
	exit(g_last_exit_status);
}

int	exec_echo(t_exec_vars *vars)
{
	int	i;

	i = 1;
	if (vars->args[i] == NULL || vars->args[i][0] == '\0')
	{
		printf("\n");
		return (0);
	}
	if (ft_strcmp(vars->args[i], "$?") == 0)
	{
		printf("%d\n", g_last_exit_status);
		return (0);
	}
	else if (ft_strcmp(vars->args[1], "-n") == 0)
		i++;
	process_args(vars->args, &(vars->error));
	if (vars->error)
		return (perror("echo: odd number of quotes\n"), 1);
	while (vars->args[i])
	{
		printf("%s", vars->args[i]);
		if (vars->args[i + 1])
			printf(" ");
		i++;
	}
	if (ft_strcmp(vars->args[1], "-n") != 0)
		printf("\n");
	return (0);
}

void	process_args(char **args, int *error)
{
	int i;
	char *new_str;

	i = 1;
	while (args[i])
	{
		new_str = handle_quotes_echo(args[i], error);
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
		return (perror("echo: memory allocation\n"), NULL);
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
				return (perror("echo: no closing quote"), free(result), NULL);
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
		return (perror("getcwd"), 1);
	if (args[1] != NULL && args[2])
		return (perror("cd: too many arguments\n"), free(cwd), 1);
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
		return (perror("getcwd"), 1);
	else
	{
		ft_putstr_fd(cwd, 1);//("%s", cwd);
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
		return (perror("getcwd"), 1);
	else
	{
		printf("%s\n", cwd);
		free(cwd);
		return (0);
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
		return (1);
	else
	{
		if (split_var(args[1], &name, &value))//think how to handle the error
			return (1);
		if (update_add_env_var(env, name, value))
			return (free(name), free(value), 1);
	}
	return (0);
}
