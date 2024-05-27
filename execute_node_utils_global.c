#include "minishell.h"

void	handle_global_env(t_parse_tree **node, char **args, int i, t_env **env)
{
	char	*str;
	char	buffer[1024] = "";
	char	*start;
	str = (*node)->data->lexeme;
	if (*(*node)->data->lexeme == 39)
	{
		args[i] = ft_strdup(str);//MEMORY LEAK
		return ;
	}
	start = str;
	while (1)
	{
		handle_dollar_sign(&start, buffer, env);
		if (ft_strchr(start, '$') == NULL)
			break ;
	}
	args[i] = ft_strdup(buffer);
}

void	handle_dollar_sign(char **start, char *buffer, t_env **env)
{
	char	*dollar;
	char	*var_start;
	char	*var_end;
	char	var_name[1024];
	char	*var_value;
	char	*start_store;

	//buffer = NULL;
	buffer[0] = '\0';
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
		//return ;
	else if (*start)
		ft_strcat(buffer, *start);
}

void	handle_quotes_global(t_parse_tree **node, char **args,
	int i, t_env **env)
{
	char	*str;
	char	buffer[1024] = "";
	char	*start;

	str = (*node)->data->lexeme; //Probably a memory leak (first quote)
	if (*(*node)->data->lexeme == 39)
	{
		args[i] = ft_strdup(str);
		if (!args[i])
			return (printf_global_error(1, 2, "echo: memory allocation\n"));//MEMORY LEAK
		return ;
	}
	start = str;
	while (1)
	{
		handle_dollar_sign(&start, buffer, env);
		if (ft_strchr(start, '$') == NULL)
			break ;
	}
	args[i] = ft_strdup(buffer);//ADD MEMORY CHECK
	if (!args[i])
			return (printf_global_error(1, 2, "echo: memory allocation\n"));
}
