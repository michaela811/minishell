#include "lexer.h"

int	exec_builtins(t_exec_vars *vars, t_env **env, char **environment)
{
	if (ft_strcmp(vars->args[0], "exit") == 0)
		return (exec_exit(vars, env));
	else if (ft_strcmp(vars->args[0], "cd") == 0)
		return (exec_cd(vars->args, env));
	else if (ft_strcmp(vars->args[0], "pwd") == 0)
		return (exec_pwd());
	else if (ft_strcmp(vars->args[0], "echo") == 0)
		return (exec_echo(vars));
	else if (ft_strcmp(vars->args[0], "export") == 0)
		return (exec_export(vars->args, env));
	else if (ft_strcmp(vars->args[0], "unset") == 0)
		return (exec_unset(vars->args, env));
	else if (ft_strcmp(vars->args[0], "env") == 0)
		return (exec_env(vars->args, environment));
	return (2);
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
	check_for_memory_leaks();
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

    result = MY_MALLOC(ft_strlen(input) + 1);
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
