#include "minishell.h"

typedef struct s_handle_vars
{
    char buffer[1024];
    char *token;
    char *delimiters;
    char **current;
    char **result;
} t_handle_vars;

// Function prototypes
void handle_single_quotes(char **current, char **result, t_exec_vars *vars);
void handle_double_quotes(char **current, char **result, t_exec_vars *vars, t_env **env);
void handle_no_quotes(t_handle_vars *local_vars, t_exec_vars *vars, t_env **env, t_parse_tree **node);


void init_handle_vars(t_handle_vars *local_vars, t_exec_vars *vars)
{
    char *empty_string = ft_strdup("");
    local_vars->result = &empty_string;
    local_vars->current = &vars->args[vars->i];
    local_vars->delimiters = "'\"";
}
// Main function
void handle_quotes_glob_1(t_parse_tree **node, t_exec_vars *vars, t_env **env)
{
    t_handle_vars local_vars;
    init_handle_vars(&local_vars, vars);

    vars->error = 0;
    while (**local_vars.current != '\0')
    {
        if (vars->inside_single_quotes)
            handle_single_quotes(local_vars.current, local_vars.result, vars);
        else if (vars->inside_double_quotes)
            handle_double_quotes(local_vars.current, local_vars.result, vars, env);
        else
            handle_no_quotes(&local_vars, vars, env, node);
        if (vars->end)
		{
			vars->end = 0;
            return;
		}
        if (vars->error)
            break;
    }
    if (!vars->error)
    {
        //free(vars->args[vars->i]);
        vars->args[vars->i] = *local_vars.result;
    }
    else
		return;
        //free(local_vars.result);
}

void handle_single_quotes(char **current, char **result, t_exec_vars *vars)
{
    char *token;

	token = *current;
    *current = ft_strchr(*current, '\'');
    if (*current == NULL)
    {
        vars->error = 1;
        return;
    }
    **current = '\0';
    *result = ft_strjoin(*result, token);
	vars->inside_single_quotes = 0;
    (*current)++;
}

void handle_double_quotes(char **current, char **result, t_exec_vars *vars, t_env **env)
{
    char buffer[1024] = "";
    char *token;

	token = *current;
    *current = ft_strchr(*current, '\"');
    if (*current == NULL)
    {
        vars->error = 1;
        return;
    }
    **current = '\0';
    if (ft_strchr(token, '$') != NULL)
    {
        handle_dollar_sign(&token, buffer, env);
        *result = ft_strjoin(*result, buffer);
    }
    else
        {*result = ft_strjoin(*result, token);}
	vars->inside_double_quotes = 0;
    (*current)++;
}

void handle_no_current(t_handle_vars *local_vars, t_exec_vars *vars, t_env **env, t_parse_tree **node)
{
    if (ft_strchr(local_vars->token, '$') != NULL)
    {
        handle_dollar_sign(&local_vars->token, local_vars->buffer, env);
        *local_vars->result = ft_strjoin(*local_vars->result, local_vars->buffer);
        if (strchr((*node)->data->lexeme, '$') != NULL && strchr(local_vars->buffer, ' '))
            vars->i = split_variable(*local_vars->result, vars->i, vars);
        else
            vars->args[vars->i] = *local_vars->result;
    }
    else
    {
        vars->args[vars->i] = ft_strjoin(*local_vars->result, local_vars->token);
    }
    vars->end = 1;
}

void handle_with_current(t_handle_vars *local_vars, t_exec_vars *vars, t_env **env, t_parse_tree **node)
{
    char delimiter = **local_vars->current;
    **local_vars->current = '\0';
    if (ft_strchr(local_vars->token, '$') != NULL)
    {
        handle_dollar_sign(&local_vars->token, local_vars->buffer, env);
        *local_vars->result = ft_strjoin(*local_vars->result, local_vars->buffer);
        if (strchr((*node)->data->lexeme, '$') != NULL && strchr(local_vars->buffer, ' '))
        {
            vars->i = split_variable(*local_vars->result, vars->i, vars);
            *local_vars->result = vars->args[vars->i];
        }
    }
    else
        *local_vars->result = ft_strjoin(*local_vars->result, local_vars->token);
    **local_vars->current = delimiter;
    if (**local_vars->current == '\'')
        vars->inside_single_quotes = 1;
    else if (**local_vars->current == '\"')
        vars->inside_double_quotes = 1;
    (*local_vars->current)++;
}

void handle_no_quotes(t_handle_vars *local_vars, t_exec_vars *vars, t_env **env, t_parse_tree **node)
{
    local_vars->token = *local_vars->current;
    *local_vars->current = ft_strpbrk(*local_vars->current, local_vars->delimiters);

    if (*local_vars->current == NULL)
    {
        handle_no_current(local_vars, vars, env, node);
        return;
    }
    handle_with_current(local_vars, vars, env, node);
}

/* void handle_no_quotes(char **current, char **result, t_exec_vars *vars, t_env **env, t_parse_tree **node)
{
    char buffer[1024] = "";
    char *token = *current;
    char *delimiters = "'\"";
    *current = strpbrk(*current, delimiters);

    if (*current == NULL)
    {
        if (ft_strchr(token, '$') != NULL)
        {
            handle_dollar_sign(&token, buffer, env);
            *result = ft_strjoin(*result, buffer);
            if (strchr((*node)->data->lexeme, '$') != NULL && strchr(buffer, ' '))
                vars->i = split_variable(*result, vars->i, vars);
            else
                vars->args[vars->i] = *result;
        }
        else
        {
            vars->args[vars->i] = ft_strjoin(*result, token);
        }
		vars->end = 1;
        return;
    }
    char delimiter = **current;
    **current = '\0';
    if (ft_strchr(token, '$') != NULL)
    {
        handle_dollar_sign(&token, buffer, env);
        *result = ft_strjoin(*result, buffer);
        if (strchr((*node)->data->lexeme, '$') != NULL && strchr(buffer, ' '))
        {
            vars->i = split_variable(*result, vars->i, vars);
            *result = vars->args[vars->i];
        }
    }
    else
        *result = ft_strjoin(*result, token);
    **current = delimiter;
	if (**current == '\'')
        vars->inside_single_quotes = 1;
    else if (**current == '\"')
        vars->inside_double_quotes = 1;
    (*current)++;
} */

void handle_quotes_glob_2(t_parse_tree **node, t_exec_vars *vars, t_env **env)
{
    char    *result = NULL;
    char    *current = vars->args[vars->i];
    char    *token;
    char    *delimiters = "'\"";
    int     inside_single_quotes = 0;
    int     inside_double_quotes = 0;
    char    buffer[1024] = "";
    char    delimiter;

    vars->error = 0;
    while (*current != '\0')
    {
        buffer[0] = '\0';
        if (inside_single_quotes)
        {
            token = current;
            current = ft_strchr(current, '\'');
            if (current == NULL)
            {
                vars->error = 1;
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
                vars->error = 1;
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
                    if (strchr((*node)->data->lexeme, '$') != NULL && ft_strchr(buffer, ' '))
                        vars->i = split_variable(result, vars->i, vars);
                    else
                        vars->args[vars->i] = result;
                }
                else
                    vars->args[vars->i] = ft_strjoin(result, token);
                return;
            }
            delimiter = *current;
            *current = '\0';
            if (strchr(token, '$') != NULL)
            {
                handle_dollar_sign(&token, buffer, env);
                result = ft_strjoin(result, buffer);
                if (strchr((*node)->data->lexeme, '$') != NULL && ft_strchr(buffer, ' '))
                {
                    vars->i = split_variable(result, vars->i, vars);//ADD ERROR HANDLING
                    result = vars->args[vars->i];
                }
            }
            else
                result = ft_strjoin(result, token);
            *current = delimiter;
            if (*current == '\'')
                inside_single_quotes = 1;
            else if (*current == '\"')
            {
                inside_double_quotes = 1;
            }
            current++;
        }
    }
    if (!vars->error)
    {
        free(vars->args[vars->i]);
        vars->args[vars->i] = result;
    }
    else
        free(result);
}