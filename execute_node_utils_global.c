#include "minishell.h"

int	ft_lennbr(int nbr)
{
	int	count;

	count = 0;
	if (nbr < 0)
	{
		count++;
		nbr *= -1;
	}
	while (nbr / 10 != 0)
	{
		count++;
		nbr = nbr / 10;
	}
	return (++count);
}

void	handle_question_mark(char **start, char *buffer, char *dollar)
{
	ft_strcat(buffer, ft_itoa(g_last_exit_status));
	*start = dollar + 2;
}

void	handle_special_chars(char **start, char *buffer, char *dollar)
{
	ft_strcat(buffer, "$");
	*start = dollar + 1;
}

void	handle_var_name(char **start, char *buffer, char *dollar, t_env **env)
{
	char	var_name[1024];
	char	*var_start;
	char	*var_end;
	char	*var_value;

	ft_memset(var_name, '\0', sizeof(var_name));
	var_start = dollar + 1;
	var_end = ft_strpbrk(var_start, " \t\n\"'$/.:");
	if (var_end == NULL)
		var_end = var_start + ft_strlen(var_start);
	ft_strncpy(var_name, var_start, var_end - var_start);
	var_name[var_end - var_start] = '\0';
	var_value = get_env_var(*env, var_name);
	if (var_value != NULL)
		ft_strcat(buffer, var_value);
	*start = var_end;
}

void	handle_dollar_sign(char **start, char *buffer, t_env **env,
			int buffer_size)
{
	char	*dollar;
	char	*start_store;

	if (start == NULL || *start == NULL)
		return ;
	ft_memset(buffer, '\0', buffer_size);
	start_store = *start;
	dollar = ft_strchr(*start, '$');
	while (dollar != NULL)
	{
		ft_strncat(buffer, *start, dollar - *start);
		if (*(dollar + 1) == '?')
			handle_question_mark(start, buffer, dollar);
		else if (*(dollar + 1) == '\0' || *(dollar + 1) == '$'
			|| *(dollar + 1) == ' ' || *(dollar + 1) == '"'
			|| *(dollar + 1) == '/')
			handle_special_chars(start, buffer, dollar);
		else
			handle_var_name(start, buffer, dollar, env);
		dollar = ft_strchr((dollar + 1), '$');
	}
	dollar = ft_strchr(start_store, '$');
	if (dollar == NULL)
		ft_strcpy(buffer, start_store);
	else if (*start)
		ft_strcat(buffer, *start);
}
