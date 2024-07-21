/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins_utils_1.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:34:19 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/16 17:15:42 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

int	export_quotes(char *input, char **output)
{
	int	len;

	len = ft_strlen(input);
	if (len == 0)
	{
		*output = ft_strdup("");
		return (0);
	}
	if ((input[0] == '"' || input[0] == '\'') && input[0] != input[len - 1])
	{
		g_last_exit_status = 1;
		return (ft_printf_fd(1, "export_quotes: unbalanced quotes"),
			g_last_exit_status);
	}
	if ((input[0] == '"' || input[0] == '\'') && input[0] == input[len - 1])
		*output = ft_strndup(input + 1, len - 2);
	else
		*output = ft_strdup(input);
	if (*output == NULL)
	{
		g_last_exit_status = 1;
		return (ft_printf_fd(1, "export_quotes: strndup error"),
			g_last_exit_status);
	}
	return (0);
}

int	update_pwd(t_env **env)
{
	char	*pwd;
	char	*cwd;

	pwd = get_env_var(*env, "PWD");//maybe add if PWD doesn't exist?
	if (!pwd)
		pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (print_err(1, 2, "my(s)hell: cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory"), 1);
	if (update_add_env_var(env, "OLDPWD", pwd))
		return (1);
	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (print_err(1, 2, "my(s)hell: cd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory"), 1);
		//return (perror("getcwd"), 1);
	if (update_add_env_var(env, "PWD", cwd))
		return (free(cwd), 1);
	return (free(cwd), 0);
}

int	change_directory_and_update(char *path, t_env **env, char **args)
{
	if (access(path, X_OK) == -1 && access(path, F_OK) == 0)
			return (print_err(1, 2, "my(s)hell: %s: %s: Permission denied\n", args[0],
					path), 1);
	if (chdir(path) != 0)
	{
		print_err(1, 2,
			"my(s)hell: %s: %s: No such file or directory\n", args[0], path);
		return (1);
	}
	if (update_pwd(env))
	{
		g_last_exit_status = 1;
		return (1);
	}
	g_last_exit_status = 0;
	return (g_last_exit_status);
}

void	exec_export_no_args(t_env *env)
{
	t_env	*current;

	current = env;
	while (current != NULL)
	{
		printf("export %s=\"%s\"\n", current->name, current->value);
		current = current->next;
	}
}
