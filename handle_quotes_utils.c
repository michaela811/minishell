#include "minishell.h"

int	check_null(void *pointer, int *error_flag)
{
	if (pointer == NULL)
	{
		*error_flag = 1;
		g_last_exit_status = 1;
		return (0);
	}
	return (1);
}

void	handle_single_quotes(char **current, char **result, t_exec_vars *vars)
{
	char	*token;

	token = *current;
	*current = ft_strchr(*current, '\'');
	if (!check_null(*current, &vars->error))
		return ;
	**current = '\0';
	*result = ft_strjoin(*result, token);
	if (!check_null(*result, &vars->error))
		return ;
	vars->inside_single_quotes = 0;
	(*current)++;
	if (!check_null(*current, &vars->end))
		return ;
}

void	handle_double_quotes_split(char **current, t_exec_vars *vars)
{
	vars->inside_double_quotes = 0;
	(*current)++;
}

void	handle_double_quotes(char **current, char **result,
			t_exec_vars *vars, t_env **env)
{
	char	buffer[1024];
	char	*token;

	ft_memset(buffer, '\0', sizeof(buffer));
	token = *current;
	*current = ft_strchr(*current, '\"');
	if (!check_null(*current, &vars->error))
		return ;
	**current = '\0';
	if (ft_strchr(token, '$') != NULL)
	{
		handle_dollar_sign(&token, buffer, env, sizeof(buffer));
		*result = ft_strjoin(*result, buffer);
		if (!check_null(*result, &vars->error))
			return ;
	}
	else
	{
		*result = ft_strjoin(*result, token);
		if (!check_null(*result, &vars->error))
			return ;
	}
	handle_double_quotes_split(current, vars);
	if (!check_null(*current, &vars->end))
		return ;
}

void	handle_quotes_final_assign(char **str1, char **str2, t_exec_vars *vars)
{
	*str1 = ft_strdup(*str2);
	check_null(*str1, &vars->error);
	free_and_null(str2);
}
