#include "minishell.h"

void init_handle_quote_redirect(t_handle_vars *local_vars, t_parse_tree **node)
{
	local_vars->result = malloc(sizeof(char *));
    *local_vars->result = ft_strdup("");
    local_vars->current = &(*node)->child->data->lexeme;
    local_vars->delimiters = "'\"";
}

void handle_quotes_glob_redirect(t_parse_tree **node, t_exec_vars *vars, t_env **env)
{
    t_handle_vars local_vars;
    init_handle_quote_redirect(&local_vars, node);

    vars->error = 0;
    //vars->end = 0;
    while (*local_vars.current != NULL && **local_vars.current != '\0')
    {
        if (vars->inside_single_quotes)
            handle_single_quotes(local_vars.current, local_vars.result, vars);
        else if (vars->inside_double_quotes)
            handle_double_quotes(local_vars.current, local_vars.result, vars, env);
        else
            handle_no_quotes_redirect(&local_vars, vars, env, node);
        if (vars->end)
		{
			vars->end = 0;
            g_last_exit_status = 0;
            return(free_and_null_double_pointer(&local_vars.result));
		}
        if (vars->error)
            break;
    }
    if (!vars->error)
		return (handle_quotes_final_assign(&(*node)->child->data->lexeme, local_vars.result, vars));
        //(*node)->child->data->lexeme = *local_vars.result;
		//return; // NOT SURE IF FREE IS ALSO NEEDED
	//free_and_null(&local_vars.result);
	free_and_null_double_pointer(&local_vars.result);
}

void handle_no_current_redirect(t_handle_vars *local_vars, t_exec_vars *vars, t_env **env, t_parse_tree **node)
{
    if (ft_strchr(local_vars->token, '$') != NULL)
    {
        handle_dollar_sign(&local_vars->token, local_vars->buffer, env, sizeof(local_vars->buffer));
        //(*node)->child->data->lexeme = ft_strjoin(*local_vars->result, local_vars->buffer);
        *local_vars->result = ft_strjoin(*local_vars->result, local_vars->buffer);
        (*node)->child->data->lexeme = ft_strjoin(*local_vars->result, local_vars->token);
		if (!check_null(*local_vars->result, &vars->error))
			return ;
    }
    else
    {
        (*node)->child->data->lexeme = ft_strjoin(*local_vars->result, local_vars->token);
        if (!check_null((*node)->child->data->lexeme, &vars->error))
			return ;
    }
    vars->end = 1;
}

void handle_with_current_redirect(t_handle_vars *local_vars, t_exec_vars *vars, t_env **env, t_parse_tree **node)
{
    char delimiter;

	delimiter = **local_vars->current;
    **local_vars->current = '\0';
	if (ft_strchr((*node)->data->lexeme, '$') != NULL)
    {
		handle_dollar_sign(&local_vars->token, local_vars->buffer, env, sizeof(local_vars->buffer));
		if (vars->error)
			return ;
    }
    else
        {
			*local_vars->result = ft_strjoin(*local_vars->result, local_vars->token);
			if (!check_null(*local_vars->result, &vars->error))
				return ;
		}
    **local_vars->current = delimiter;
    if (**local_vars->current == '\'')
        vars->inside_single_quotes = 1;
    else if (**local_vars->current == '\"')
        vars->inside_double_quotes = 1;
    (*local_vars->current)++;
}

void handle_no_quotes_redirect(t_handle_vars *local_vars, t_exec_vars *vars, t_env **env, t_parse_tree **node)
{
    local_vars->token = *local_vars->current;
    *local_vars->current = ft_strpbrk(*local_vars->current, local_vars->delimiters);
    if (*local_vars->current == NULL)
    {
        handle_no_current_redirect(local_vars, vars, env, node);
        return;
    }
    handle_with_current_redirect(local_vars, vars, env, node);
}

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
                handle_dollar_sign(&token, buffer, env, sizeof(buffer));
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
                    handle_dollar_sign(&token, buffer, env, sizeof(buffer));
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
                handle_dollar_sign(&token, buffer, env, sizeof(buffer));
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
