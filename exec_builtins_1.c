#include "lexer.h"

int exec_builtins(char **args, t_env **env, char **environment)
{
    if (ft_strcmp(args[0], "cd") == 0)
        return(exec_cd(args, env));
    else if (ft_strcmp(args[0], "pwd") == 0)
        return(exec_pwd(args));
    else if (ft_strcmp(args[0], "echo") == 0)
        return(exec_echo(args), 0);
    else if (ft_strcmp(args[0], "export") == 0)
        return(exec_export(args, env));
    else if (ft_strcmp(args[0], "unset") == 0)
        return(exec_unset(args, env));
    else if (ft_strcmp(args[0], "env") == 0)
        return(exec_env(args, env, environment));
    return (2);
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

int	exec_pwd(char **args)
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

void	exec_echo(char **args)
{
	int	i;

	i = 1;
	if (args[i] == NULL)
	{
		printf("\n");
		return ;
	}
	if (ft_strcmp(args[i], "$?") == 0)
	{
		printf("%d\n", g_last_exit_status);
		return ;
	}
	else if (ft_strcmp(args[1], "-n") == 0)
		i++;
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (ft_strcmp(args[1], "-n") != 0)
		printf("\n");
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
