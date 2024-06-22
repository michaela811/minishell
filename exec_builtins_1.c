#include "minishell.h"

int	exec_builtins(t_exec_vars *vars, t_free_data *exec_data)
{
	if (ft_strcmp(vars->args[0], "exit") == 0)
		return (exec_exit(vars, exec_data));
	else if (ft_strcmp(vars->args[0], "cd") == 0)
		return (exec_cd(vars->args, &exec_data->env));//, exec_data->line));
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

/*int	ft_atoi_no_minus(const char *nptr)
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
}*/

int	overflow_check(char *result)
{
	//char	*check;
	char	*end;

	while (isspace((unsigned char)*result))
        result++;
	//int is_negative = (*result == '-');
	end = result;
	if (*result == '+' || *result == '-')
		end++;
	while (*result == '0')
		result++;
	//end = result;
	while (ft_isdigit((unsigned char)*end))
    	end++;
	//long long num = strtoll(result, NULL, 10);
	/*check = end;
	while (*check != '\0')
	{
        if (!ft_isspace((unsigned char)*check))
            return 0;
        check++;
    }
	*end = '\0';*/
    while (*end != '\0') {
        if (!isspace((unsigned char)*end)) {
            return 0;  // Invalid if any non-space characters after the number
        }
        end++;
    }
	char *num = ft_strtok(result, " ");
	/*long long num = strtoll(result, NULL, 10);
	if (is_negative)
	{
        if (num < INT_MIN && num != LLONG_MIN)
            return 1;
    }
	else
	{
        if (num > INT_MAX && num != LLONG_MAX)
            return 1;
    }*/
	if (ft_atoi(num) == INT_MAX && (ft_strcmp(num, "2147483647") != 0
	|| ft_strcmp(num, "9223372036854775807") != 0))
		return (1);
	return (0);
}

int ft_isspace(char c)
{
    return c == ' '  ||
           c == '\f' ||
           c == '\n' ||
           c == '\r' ||
           c == '\t' ||
           c == '\v';
}

int is_string_numeric(const char *str)
{
	while (ft_isspace((unsigned char)*str))
        str++;
	if (*str == '+' || *str == '-')
		str++;
	if (!*str)
    	return (0);
    while (ft_isdigit(*str))
	{
        str++;
	}
	while (*str)
	{
        if (!ft_isspace((unsigned char)*str))
            return (0);
        str++;
    }
    return (1);
}

int	exec_exit(t_exec_vars *vars, t_free_data *exec_data)
{
	int	i;
	char *result;

	i = 0;
	result = NULL;
	if (vars->args[1] != NULL)
	{
		result = vars->args[1];
		if (result[i] == '+' || result[i] == '-')
			i++;
		while (result[i] == '0')
			i++;
		i = 0;
		if (!is_string_numeric(result) || overflow_check(result))
			return (printf_global_error(2, 2, "my(s)hell: %s: %s: numeric argument required\n", vars->args[0], vars->args[1]), g_last_exit_status);
		if (result[0] == '\0' && ft_strlen(result) == 0)
			return (printf_global_error(2, 2, "my(s)hell: %s: %s: numeric argument required\n", vars->args[0], vars->args[1]), g_last_exit_status);
		if (vars->args[2] != NULL)
			return (printf_global_error(1, 2, "my(s)hell: %s: too many arguments\n", vars->args[0]), g_last_exit_status);
		g_last_exit_status = ft_atoi(result);
	}
	/*if (vars->args[1] != NULL && !g_last_exit_status)
	{
		printf_global_error(2, 2, "my(s)hell: exit: %s: numeric argument required\n", vars->args[1]);
		return(g_last_exit_status);
	}*/
	free_exit_data(exec_data);
	free_env_array(vars->args);
	free(vars);
	exit(g_last_exit_status);
}

int echo_n_control (char *arg)
{
	int i;

	i = 1;
	if (arg[0] == '-' && arg[i] == 'n')
	{
		i++;
		while (arg[i] == 'n')
			i++;
		if (arg[i] == '\0')
			return (1);
	}
	return (0);
}

int	exec_echo(t_exec_vars *vars)
{
	int	i;

	i = 1;
	if (vars->args[1] == NULL)
		return (printf_global_error(g_last_exit_status, vars->fd_out, "\n"), 1);
	while (vars->args[i] && echo_n_control(vars->args[i]))
		i++;
	while (vars->args[i])
	{
	   	printf_global_error(g_last_exit_status, vars->fd_out, "%s", vars->args[i]);
	    if (vars->args[i + 1])
	        printf_global_error(g_last_exit_status, vars->fd_out, " ");
	    i++;
	}
	if (echo_n_control(vars->args[1]) == 0)
		printf_global_error(g_last_exit_status, vars->fd_out, "\n");
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

void return_exit_status(char *result, int *i, int *j, int *error)
{
    char *temp;

	temp = ft_itoa(g_last_exit_status);
    if (temp == NULL)
    {
        *error = 1;
        printf_global_error(1, 2, "echo: memory allocation\n");
        return ;
    }
    result[*j] = temp[0];
    *j = *j + ft_strlen(temp);
	*i += 2;
    free(temp);
    return ;
}

int	handle_quotes_in_echo(char *input, char *result, int *i, int *j)
{
    char quote;

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
int malloc_result(char **result, char *input, int *error)
{
    *result = malloc(ft_strlen(input) + 1);
    if (!(*result))
    {
        *error = 1;
        return (1);
    }
    return (0);
}

char *handle_quotes_echo(char *input, int *error)
{
	char *result;
	int i;
	int j;

	if (malloc_result(&result, input, error))
		return (printf_global_error(1, 2, "echo: memory allocation\n"), NULL);
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
		return (printf_global_error(1, 2, "getcwd\n"), g_last_exit_status);
	if (args[1] != NULL && args[2])
		return (printf_global_error(1, 2, "my(s)hell: %s: too many arguments\n", args[0]), free(cwd), g_last_exit_status);
	else if (args[1] == NULL || ft_strcmp(args[1], "~") == 0)
		return (change_directory_and_update(get_env_var(*env, "HOME"),
				env, cwd, args));
	else if (ft_strcmp(args[1], "..") == 0)
		return (change_directory_and_update("..", env, cwd, args));
	else if (ft_strcmp(args[1], "-") == 0)
	{
		if (get_env_var(*env, "OLDPWD") == NULL)
			return (printf_global_error(1, 2, "my(s)hell: %s: OLDPWD not set\n", args[0]), g_last_exit_status);
		change_directory_and_update(get_env_var(*env, "OLDPWD"), env, cwd, args);
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
		return (printf_global_error(1, 2, "getcwd\n"), g_last_exit_status);
	else
	{
		ft_putstr_fd(cwd, 1);
		ft_putstr_fd(": Is a directory\n", 1);
		g_last_exit_status = 1;
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
int split_to_name_value(char **args, char **name, char **value, int *i)
{
	if (split_var(args[*i], name, value))
	{
		g_last_exit_status = 1;
		(*i)++;
		return (1);
	}
	return (0);
}
int exec_update_add_env_var(t_env **env, char *name, char *value)
{
	if (update_add_env_var(env, name, value))
	{
		free(name);
		free(value);
		g_last_exit_status = 1;
		return (1);
	}
	return (0);
}

void init_free_name_value(char **name, char **value, int i)
{
	if (i == 1)
	{
		*name = NULL;
		*value = NULL;
	}
	else
	{
		free(*name);
		free(*value);
	}
}

int	exec_export(char **args, t_env **env)
{
	char	*name;
	char	*value;
	int		control;
	int		i;

	i = 1;
	init_free_name_value(&name, &value, i);
	if (args[1] == NULL)
		return (exec_export_no_args(*env), 0);
	while (args[i] != NULL)
	{
		control = var_control(args[0], args[1]);
		if (control == 1)
			return (g_last_exit_status);
		else
		{
			if (split_to_name_value(args, &name, &value, &i))
				continue ;
			if (exec_update_add_env_var(env, name, value))
				continue ;
			init_free_name_value(&name, &value, ++i);
		}
	}
	g_last_exit_status = 0;
	return (g_last_exit_status);
}

/*
int	exec_export(char **args, t_env **env)
{
	char	*name;
	char	*value;
	int		control;
	int		i;

	i = 1;
	name = NULL;
	value = NULL;
	if (args[1] == NULL)
		return (exec_export_no_args(*env), 0);
	while (args[i] != NULL)
	{
		control = var_control(args[0], args[1]);
		if (control == 1)
			return (g_last_exit_status);
		else
		{
				if (split_var(args[i], &name, &value))
			{
				g_last_exit_status = 1;
				i++;
				continue;
			}
			if (update_add_env_var(env, name, value))
			{
				free(name);
				free(value);
				g_last_exit_status = 1;
				continue;
			}
			free(name);
			free(value);
			i++;
		}
	}
	g_last_exit_status = 0;
	return (g_last_exit_status);
} */

