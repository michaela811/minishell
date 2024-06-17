#include "minishell.h"

/* void	handle_global_env(t_parse_tree **node, char **args, int i, t_env **env)
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
} */

int	ft_lennbr(int nbr)
{
	int count;

	count = 0;
	if (nbr < 0)
	{
		count++;
		nbr *= -1;
	}
	while (nbr/10 != 0)
	{
		count++;
		nbr = nbr/10;
	}
	return(++count);
}

/*void append_int_to_buffer_with_marker(char *buffer, int *buf_size, int num)
{
    strcat(buffer, "[INT]");
    *buf_size += strlen("[INT]");
    memcpy(buffer + *buf_size, &num, sizeof(int));
    *buf_size += sizeof(int);
}

void handle_dollar_sign(char **start, char *buffer, t_env **env) {
    char *dollar;
    char *var_start;
    char *var_end;
    char var_name[1024];
    char *var_value;
    char *start_store;
    int buf_size = 0;
    char temp_buffer[1024] = ""; // Temporary buffer for intermediate string operations

    buffer[0] = '\0';
    start_store = *start;

    while ((dollar = strchr(*start, '$')) != NULL) {
        strncat(temp_buffer, *start, dollar - *start);
        if (*(dollar + 1) == '?') {
            // Copy current temp_buffer to main buffer before appending binary data
            strcat(buffer, temp_buffer);
            buf_size += strlen(temp_buffer);

            // Append the integer in binary form with a marker
            append_int_to_buffer_with_marker(buffer, &buf_size, g_last_exit_status);
            *start = dollar + 2;
        } else if (*(dollar + 1) == '\0' || *(dollar + 1) == '$' || *(dollar + 1) == ' ' || *(dollar + 1) == '"') {
            strcat(temp_buffer, "$");
            *start = dollar + 1;
        } else {
            var_start = dollar + 1;
            var_end = strpbrk(var_start, " \t\n\"'$/");
            if (var_end == NULL)
                var_end = var_start + strlen(var_start);
            strncpy(var_name, var_start, var_end - var_start);
            var_name[var_end - var_start] = '\0';
            var_value = get_env_var(*env, var_name);
            if (var_value != NULL)
                strcat(temp_buffer, var_value);
            *start = var_end;
        }
    }

    // Copy remaining temp_buffer to main buffer
    strcat(buffer, temp_buffer);
    buf_size += strlen(temp_buffer);

    if ((dollar = strchr(start_store, '$')) == NULL) {
        strcpy(buffer, start_store); // Probably use strncpy!
    } else if (*start) {
        strcat(buffer, *start);
    }
}*/

void	handle_dollar_sign(char **start, char *buffer, t_env **env)//, int *k)
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
		else if (*(dollar + 1) == '\0' || *(dollar + 1) == '$' || *(dollar + 1) == ' ' || *(dollar + 1) == '"' || *(dollar + 1) == '/')
		{
			ft_strcat(buffer, "$");
			*start = dollar + 1;
		}
		else
		{
			var_start = dollar + 1;
			var_end = ft_strpbrk(var_start, " \t\n\"'$/");
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
		ft_strcpy(buffer, start_store);
	else if (*start)
		ft_strcat(buffer, *start);
}

/*void handle_dollar_sign(char **start, char *buffer, t_env **env) {
    char *dollar;
    char *var_start;
    char *var_end;
    char var_name[1024];
    char *var_value;
    char *start_store;
    int buf_size = 0;
    char temp_buffer[1024] = ""; // Temporary buffer for intermediate string operations

    buffer[0] = '\0';
    start_store = *start;

    while ((dollar = strchr(*start, '$')) != NULL) {
        strncat(temp_buffer, *start, dollar - *start);
        if (*(dollar + 1) == '?') {
            // Copy current temp_buffer to main buffer before appending binary data
            strcat(buffer, temp_buffer);
            buf_size += strlen(temp_buffer);

            // Append the integer in binary form
            append_int_to_buffer(buffer, &buf_size, g_last_exit_status);
            *start = dollar + 2;
        } else if (*(dollar + 1) == '\0' || *(dollar + 1) == '$' || *(dollar + 1) == ' ' || *(dollar + 1) == '"') {
            strcat(temp_buffer, "$");
            *start = dollar + 1;
        } else {
            var_start = dollar + 1;
            var_end = strpbrk(var_start, " \t\n\"'$/");
            if (var_end == NULL)
                var_end = var_start + strlen(var_start);
            strncpy(var_name, var_start, var_end - var_start);
            var_name[var_end - var_start] = '\0';
            var_value = get_env_var(*env, var_name);
            if (var_value != NULL)
                strcat(temp_buffer, var_value);
            *start = var_end;
        }
    }

    // Copy remaining temp_buffer to main buffer
    strcat(buffer, temp_buffer);
    buf_size += strlen(temp_buffer);

    if ((dollar = strchr(start_store, '$')) == NULL) {
        strcpy(buffer, start_store); // Probably use strncpy!
    } else if (*start) {
        strcat(buffer, *start);
    }
}*/

/* void	handle_quotes_global(t_parse_tree **node, char **args,
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
} */
