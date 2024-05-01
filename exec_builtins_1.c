#include "lexer.h"

int	exec_builtins(t_exec_vars *vars, t_env **env, char **environment)
{
	if (ft_strcmp(vars->args[0], "exit") == 0)
		return (exec_exit(vars->args, env));
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

int	exec_exit(char **args, t_env **env)
{
	int	i;

	i = 0;
	if (args[1] != NULL)
	{
		if (args[2] != NULL)
			return (printf("exit: too many arguments"));
		while (args[1][i])
		{
			if (ft_isdigit(args[1][i]) == 0)
			{
				g_last_exit_status = 156;
				return (156);
			}
			i++;
		}
		g_last_exit_status = ft_atoi(args[1]);
	}
	free_env(*env);
	exit(0);
}

int	exec_echo(t_exec_vars *vars)
{
	int	i;

	i = 1;
	if (vars->args[i] == NULL)
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
		new_str = remove_even_quotes(args[i], error);
		if (*error)
		{
			free(new_str);
			return ;
		}
		free(args[i]);
        args[i] = new_str;
        i++;
	}
}

char	*remove_even_quotes(char *str, int *error)
{
	int		quote_count;
	int		i;
	char	*new_str;
	int		j;

	quote_count = 0;
	i = 0;
	while (str[i] != '\0')
    {
        if (str[i] == '"' || str[i] == '\'')
            quote_count++;
		i++;
    }
    if (quote_count % 2 != 0)
	{
		*error = 1;
        return (NULL);
	}
    new_str = malloc(ft_strlen(str) + 1);
    j = 0;
	i = 0;
    while (str[i] != '\0')
    {
        if (str[i] != '"' && str[i] != '\'')
            new_str[j++] = str[i];
		i++;
    }
    new_str[j] = '\0';
    return (new_str);
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
	if (control == 2)
	{
		if (split_var(args[1], &name, &value))//think how to handle the error
			return (1);
		if (update_add_env_var(env, name, value))
			return (free(name), free(value), 1);
	}
	return (0);
}
