#include "minishell.h"

void	init_exec_vars(t_exec_vars *vars)
{
	int	i;

	vars->fd_in = 0;
	vars->fd_out = 1;
	vars->args = malloc(10 * sizeof(char *));  // Allocate memory for args
    if (!vars->args)
    {
        // Handle memory allocation error
        return;
    }
	i = 0;
	while (i < 10)
		vars->args[i++] = NULL;
	vars->i = 0;
	vars->error = 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* void handle_dollar_sign(char **token, char *buffer, t_env **env) {
    // Implement your logic here for handling the dollar sign.
} */

/* char *ft_strpbrk(const char *s, const char *accept) {
    return strpbrk(s, accept);  // Placeholder implementation
} */

/* char *ft_strtok(char *str, const char *delim) {
    return strtok(str, delim);  // Placeholder implementation
} */

/* char *ft_strjoin(char *str1, const char *str2) {
    if (!str1) return strdup(str2);
    if (!str2) return strdup(str1);

    char *result = malloc(strlen(str1) + strlen(str2) + 1);
    if (!result) return NULL;

    strcpy(result, str1);
    strcat(result, str2);
    free(str1);
    return result;
} */

void handle_quotes_glob(char **arg, t_env **env, int *error)
{
    char    *result = NULL;
    char    *current = *arg;
    char    *token;
    char    *delimiters = "'\"";
    int     inside_single_quotes = 0;
    int     inside_double_quotes = 0;
    char    buffer[1024] = "";
    char    delimiter;

    *error = 0;

    while (*current != '\0')
    {
        //memset(buffer, 0, sizeof(buffer)); //this function is not allowed
        buffer[0] = '\0';
        if (inside_single_quotes)
        {
            token = current;
            current = ft_strchr(current, '\'');
            if (current == NULL)
            {
                *error = 1; // Error: unmatched single quote
                break;
            }
            *current = '\0';
            result = ft_strjoin(result, token);
            inside_single_quotes = 0;
            current++;
        }
        else if (inside_double_quotes)
        {
            token = current;
            current = ft_strchr(current, '\"');
            if (current == NULL)
            {
                *error = 1; // Error: unmatched double quote
                break;
            }
            *current = '\0';
            if (ft_strchr(token, '$') != NULL)
            {
                handle_dollar_sign(&token, buffer, env);
                result = ft_strjoin(result, buffer);
            }
            else
                result = ft_strjoin(result, token);
            inside_double_quotes = 0;
            current++;
        }
        else
        {
            token = current;
            current = strpbrk(current, delimiters);
            if (current == NULL)
            {
                if (strchr(token, '$') != NULL)
                {
                    handle_dollar_sign(&token, buffer, env);
                    result = ft_strjoin(result, buffer);
                }
                else
                    result = ft_strjoin(result, token);
                break;
            }
            delimiter = *current;
            *current = '\0';
            if (strchr(token, '$') != NULL)
            {
                handle_dollar_sign(&token, buffer, env);
                result = ft_strjoin(result, buffer);
            }
            else
                result = ft_strjoin(result, token);
            *current = delimiter; // Restore the delimiter
            if (*current == '\'')
                inside_single_quotes = 1;
            else if (*current == '\"')
            {
                inside_double_quotes = 1;
            }
            current++;
        }
    }
    if (!*error)
    {
        free(*arg);
        *arg = result;
    }
    else
        free(result); // Free the partially constructed result in case of error
}

/* void handle_quotes_glob(char **arg, t_env **env, int *error) {
    char *result = NULL;
    char *token;
    //char *next_token;
    char *delimiters = "'\"";
    int inside_single_quotes = 0;
    int inside_double_quotes = 0;
    char buffer[1024] = "";

    *error = 0;
    char *quote_ptr = ft_strpbrk(*arg, delimiters);
            if (quote_ptr != NULL) {
                if (*quote_ptr == '\'') {
                    inside_single_quotes = 1;
                } else if (*quote_ptr == '"') {
                    inside_double_quotes = 1;
                }
            }
    else// (!inside_double_quotes && !inside_single_quotes)
        token = ft_strtok(*arg, delimiters);
    while (1)
        {
            if (inside_single_quotes) {
            // We are inside single quotes, so look for the closing single quote
            token = ft_strtok(*arg, "'");
            if (token == NULL) {
                *error = 1; // Error: unmatched single quote
                break;
            }
            result = ft_strjoin(result, token);
            inside_single_quotes = 0; // Reset single quote flag
        } else if (inside_double_quotes) {
            // We are inside double quotes, so look for the closing double quote
            token = ft_strtok(*arg, "\"");
            if (token == NULL) {
                *error = 1; // Error: unmatched double quote
                break;
            }
            if (strchr(token, '$') != NULL) {
                handle_dollar_sign(&token, buffer, env);
                result = ft_strjoin(result, buffer);
            } else {
                result = ft_strjoin(result, token);
            }
            inside_double_quotes = 0; // Reset double quote flag
        } else if (result || token) {
            // We are not inside any quotes, so process the token
            if (token && ft_strchr(token, '$') != NULL) {
                handle_dollar_sign(&token, buffer, env);
                result = ft_strjoin(result, buffer);
            } else {
                result = ft_strjoin(result, token);
            }
        }

        // Check if we are starting a quoted section
        if (token != NULL) {
            char *quote_ptr = ft_strpbrk(token, delimiters);
            if (quote_ptr != NULL) {
                if (*quote_ptr == '\'') {
                    inside_single_quotes = 1;
                } else if (*quote_ptr == '"') {
                    inside_double_quotes = 1;
                }
            }
        }
        token = ft_strtok(NULL, delimiters);
        if (!token)
            break;
    }

    if (!*error) {
        free(*arg);
        *arg = result;
    } else {
        free(result); // Free the partially constructed result in case of error
    }
}
 */

/* void	handle_quotes_glob(char **arg, t_env **env, int *error)
{
    char *result;
    int k;
    int j;
    int count;
    char quote;
    //char *start_arg = *arg;
    char *start;
    char	buffer[1024] = "";

    *error = 0;
    result = malloc(ft_strlen(*arg) + 1);
    if (!result)
    {
        *error = 1;
        return (printf_global_error(1, 2, "echo: memory allocation\n"));
    }
    k = 0;
    j = 0;
    while ((*arg)[k] != '\0')
    {
        if ((*arg)[k] == '\'' || (*arg)[k] == '\"')
        {
            quote = (*arg)[k++];
            start = &(*arg)[k];
            count = 0;
            while ((*arg)[k] != '\0' && (*arg)[k] != quote)
            {
                result[j++] = (*arg)[k++];
                count++;
            }
            if ((*arg)[k] != quote)
            {
                *error = 1;
                g_last_exit_status = 1;
                return(free(result));
            }
            //if (start[j] == '"')
                start[count] = '\0';
                //printf("result: %s\n",result);
            if (quote == '\"' && ft_strchr(start, '$') != NULL)
            {
                handle_dollar_sign(&start, buffer, env, &k);
                if (result)
                    ft_strjoin(result, buffer);
                else
                {
                    free(result);
                    result = malloc(ft_strlen(buffer) + 1);
                    if (!result)
                    {
                        *error = 1;
                        return (printf_global_error(1, 2, "echo: memory allocation\n"));
                    }
                    ft_strcpy(result, buffer);
                }
                j = ft_strlen(result);
            }
            k++;
        }
        else if ((*arg)[k] == '$')
        {
            start = &(*arg)[k];
            handle_dollar_sign(&start, buffer, env, &k);
            j += ft_strlen(buffer);
            //if (start)
		        //ft_strcat(buffer, start);
            if (result)
                result = ft_strjoin(result, buffer);
            else
            {
                free(result);
                result = malloc(ft_strlen(buffer) + 1);
                if (!result)
                {
                    *error = 1;
                    return (printf_global_error(1, 2, "echo: memory allocation\n"));
                }
                ft_strcpy(result, buffer);
            }
            //j = ft_strlen(result);
            //k = ft_strlen(*arg);
            //k = start_arg - start;
            k++;
        }
        else
            result[j++] = (*arg)[k++];
    }
    result[j] = '\0';
    free(*arg);
    *arg = result;
} */

void	handle_node_data(t_parse_tree **node, t_exec_vars *vars, t_env **env)
{
    int index;
	if ((*node)->data->type == RED_FROM || (*node)->data->type == RED_TO
		|| (*node)->data->type == APPEND || (*node)->data->type == HERE_DOC)
	    return (handle_redirection(node, vars, env));
    vars->args[vars->i] = ft_strdup((*node)->data->lexeme);
	if (!vars->args[vars->i])
	{
		vars->error = 1;
		return(printf_global_error(1, 2, "my(s)hell: ft_strdup error\n"));
	}
    index = vars->i;
    if ((*node)->data->lexeme[0] == '$' && ft_strchr(vars->args[vars->i], ' ') && !ft_strchr(vars->args[vars->i], '\''))
        vars->i = split_variable(vars->args[vars->i], vars->i, vars);//ADD ERROR HANDLING
    while (index <= vars->i)
    {
        handle_quotes_glob(&vars->args[index], env, &vars->error);
        if (vars->error)
        {
            g_last_exit_status = 1;
            return ;
        }
        index++; //needed? Im not sure
    }
    vars->i++;
}

/* void	handle_node_data(t_parse_tree **node, t_exec_vars *vars, t_env **env)
{
	if ((*node)->data->type == RED_FROM || (*node)->data->type == RED_TO
		|| (*node)->data->type == APPEND || (*node)->data->type == HERE_DOC)
	{
		handle_redirection(node, vars);
		return ;
	}
	if ((*node)->data->lexeme[0] == '$' && strcmp((*node)->data->lexeme, "$?") == 0)
	{
		vars->args[vars->i] = ft_strdup(ft_itoa(g_last_exit_status));
		if (!vars->args[vars->i])
		{
			vars->error = 1;
			return(printf_global_error(1, 2, "my(s)hell: ft_strdup error\n"));
		}
	}
	else if ((*node)->data->lexeme)
	{
		//hanlde_quotes_glob(node, vars->args, vars->i, env);
		handle_quotes_global(node, vars->args, vars->i, env);
		if (g_last_exit_status)
		{
			vars->error = 1;
			return ;
		}
		if (ft_strcmp(vars->args[vars->i], "") == 0)
			return ;
	}
	else
	{
		vars->args[vars->i] = ft_strdup((*node)->data->lexeme);
		if (!vars->args[vars->i])
		{
			vars->error = 1;
			return(printf_global_error(1, 2, "my(s)hell: ft_strdup error\n"));
		}
	}
	if ((*node)->data->lexeme[0] == '$' && ft_strchr(vars->args[vars->i], ' '))
		vars->i = split_variable(vars->args[vars->i], vars->i, vars);//ADD ERROR HANDLING
	vars->i++;
} */

int split_variable(char *arg, int i, t_exec_vars *vars)
{
    char **split_args;
    int j;

    j = 0;
    split_args = ft_split(arg, ' ');
    if (!split_args)
        return i;
    while (split_args[j])
	{
        vars->args[i + j] = split_args[j];
        j++;
    }
    free(split_args);
    return i + (j - 1);
}

/*int split_variable(char *arg, int i, t_exec_vars *vars)
{
    char **split_args;
    int j;

    j = 0;
    split_args = ft_split(arg, ' ');
    if (!split_args)
        return i;
    while (split_args[j])
	{
        vars->args[i + j] = split_args[j];
        j++;
    }
    free(split_args);
    return i + (j - 1);
}*/

void	handle_redirection(t_parse_tree **node, t_exec_vars *vars, t_env **env)
{
	if ((*node)->data->type == RED_FROM)
		return (handle_redirection_from(node, vars, env));
	else if ((*node)->data->type == RED_TO)
		return (handle_redirection_to(node, vars, env));
	else if ((*node)->data->type == APPEND)
		return (handle_redirection_append(node, vars, env));
	else if ((*node)->data->type == HERE_DOC)
		return (handle_redirection_here_doc(node, vars));//, env));
}
