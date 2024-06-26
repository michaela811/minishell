#include "minishell.h"

void	init_handle_quote_redirect(t_handle_vars *l_vars, t_parse_tree **node)
{
	l_vars->result = malloc(sizeof(char *));
	*l_vars->result = malloc(1);
	**l_vars->result = '\0';
	l_vars->current = &(*node)->child->data->lexeme;
	l_vars->delimiters = "'\"";
}

void	handle_quotes_glob_redirect(t_parse_tree **node, t_exec_vars *vars, t_env **env)
{
	t_handle_vars l_vars;
	init_handle_quote_redirect(&l_vars, node);

	vars->error = 0;
	while (*l_vars.current != NULL && **l_vars.current != '\0')
	{
		if (vars->inside_single_quotes)
			handle_single_quotes(l_vars.current, l_vars.result, vars);
		else if (vars->inside_double_quotes)
			handle_double_quotes(l_vars.current, l_vars.result, vars, env);
		else
			handle_no_quotes_redirect(&l_vars, vars, env, node);
		if (vars->end)
		{
			vars->end = 0;
			g_last_exit_status = 0;
			return (free_and_null_double_pointer(&l_vars.result));
		}
		if (vars->error)
			break ;
	}
	if (!vars->error)
		return (handle_quotes_final_assign(&(*node)->child->data->lexeme,
				l_vars.result, vars));
	return (free_and_null_double_pointer(&l_vars.result));
}

void	handle_no_current_redirect(t_handle_vars *l_vars, t_exec_vars *vars, t_env **env, t_parse_tree **node)
{
	if (ft_strchr(l_vars->token, '$') != NULL)
	{
		handle_dollar_sign(&l_vars->token, l_vars->buffer, env, sizeof(l_vars->buffer));
		*l_vars->result = ft_strjoin(*l_vars->result, l_vars->buffer);
		if (!check_null(*l_vars->result, &vars->error))
			return ;
		(*node)->child->data->lexeme = ft_strdup(*l_vars->result);
	}
	else
	{
		(*node)->child->data->lexeme = ft_strjoin(*l_vars->result, l_vars->token);
		if (!check_null((*node)->child->data->lexeme, &vars->error))
			return ;
	}
	vars->end = 1;
}

void	handle_with_current_redirect(t_handle_vars *l_vars, t_exec_vars *vars, t_env **env, t_parse_tree **node)
{
	char	delimiter;

	delimiter = **l_vars->current;
	**l_vars->current = '\0';
	if (ft_strchr((*node)->data->lexeme, '$') != NULL)
	{
		handle_dollar_sign(&l_vars->token, l_vars->buffer, env, sizeof(l_vars->buffer));
		if (vars->error)
			return ;
	}
	else
	{
		*l_vars->result = ft_strjoin(*l_vars->result, l_vars->token);
		if (!check_null(*l_vars->result, &vars->error))
			return ;
	}
	**l_vars->current = delimiter;
	if (**l_vars->current == '\'')
		vars->inside_single_quotes = 1;
	else if (**l_vars->current == '\"')
		vars->inside_double_quotes = 1;
	(*l_vars->current)++;
}

void	handle_no_quotes_redirect(t_handle_vars *l_vars, t_exec_vars *vars, t_env **env, t_parse_tree **node)
{
	l_vars->token = *l_vars->current;
	*l_vars->current = ft_strpbrk(*l_vars->current, l_vars->delimiters);
	if (*l_vars->current == NULL)
	{
		handle_no_current_redirect(l_vars, vars, env, node);
		return ;
	}
	handle_with_current_redirect(l_vars, vars, env, node);
}

void	handle_quotes_glob(char **arg, t_env **env, int *error)
{
	char	*result = NULL;
	char	*current = *arg;
	char	*token;
	char	*delimiters = "'\"";
	int		inside_single_quotes = 0;
	int		inside_double_quotes = 0;
	char	buffer[1024] = "";
	char	delimiter;

	*error = 0;
	while (*current != '\0')
	{
		buffer[0] = '\0';
		if (inside_single_quotes)
		{
			token = current;
			current = ft_strchr(current, '\'');
			if (current == NULL)
			{
				*error = 1;
				break ;
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
				*error = 1;
				break ;
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
				break ;
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
	if (!*error)
	{
		free(*arg);
		*arg = result;
	}
	else
		free(result);
}
