#include "minishell.h"

void	init_handle_vars(t_handle_vars *l_vars, t_exec_vars *vars)
{
	l_vars->result = malloc(sizeof(char *));
	*l_vars->result = ft_strdup("");
	l_vars->current = &vars->args[vars->i];
	l_vars->delimiters = "'\"";
}

// Main function
void	handle_quotes_glob_1(t_parse_tree **node, t_exec_vars *vars,
		t_env **env)
{
	t_handle_vars	l_vars;

	init_handle_vars(&l_vars, vars);
	vars->error = 0;
	while (**l_vars.current != '\0')
	{
		if (vars->inside_single_quotes)
			handle_single_quotes(l_vars.current, l_vars.result, vars);
		else if (vars->inside_double_quotes)
			handle_double_quotes(l_vars.current, l_vars.result, vars,
				env);
		else
			handle_no_quotes(&l_vars, vars, env, node);
		if (vars->end)
		{
			vars->end = 0;
			return (free_and_null_double_pointer(&l_vars.result));
		}
		if (vars->error)
			break ;
	}
	if (!vars->error)
		return (handle_quotes_final_assign(&vars->args[vars->i],
				l_vars.result, vars));
	free_and_null_double_pointer(&l_vars.result);
}

void	handle_no_current(t_handle_vars *l_vars, t_exec_vars *vars,
		t_env **env, t_parse_tree **node)
{
	if (ft_strchr(l_vars->token, '$') != NULL)
	{
		handle_dollar_sign(&l_vars->token, l_vars->buffer, env,
			sizeof(l_vars->buffer));
		*l_vars->result = ft_strjoin(*l_vars->result,
				l_vars->buffer);
		if (!check_null(*l_vars->result, &vars->error))
			return ;
		if (strchr((*node)->data->lexeme, '$') != NULL
			&& strchr(l_vars->buffer, ' '))
		{
			vars->i = split_variable(*l_vars->result, vars->i, vars);
			if (vars->error)
				return (free(*l_vars->result));
		}
		else
			vars->args[vars->i] = ft_strdup(*l_vars->result);
	}
	else
	{
		vars->args[vars->i] = ft_strjoin(*l_vars->result, l_vars->token);
		if (!check_null(vars->args[vars->i], &vars->error))
			return ;
	}
	vars->end = 1;
}

void	handle_with_current_dollar(t_handle_vars *l_vars,
			t_exec_vars *vars, t_env **env, t_parse_tree **node)
{
	handle_dollar_sign(&l_vars->token, l_vars->buffer, env,
		sizeof(l_vars->buffer));
	*l_vars->result = ft_strjoin(*l_vars->result,
			l_vars->buffer);
	if (!check_null(*l_vars->result, &vars->error))
		return ;
	if (ft_strchr((*node)->data->lexeme, '$') != NULL
		&& ft_strchr(l_vars->buffer, ' '))
	{
		vars->i = split_variable(*l_vars->result, vars->i, vars);
		if (vars->error)
			return (free(*l_vars->result));
		free(*l_vars->result);//check this
		*l_vars->result = ft_strdup(vars->args[vars->i]);//CHECK THIS
	}
}

void	handle_with_current(t_handle_vars *l_vars,
		t_exec_vars *vars, t_env **env, t_parse_tree **node)
{
	char	delimiter;

	delimiter = **l_vars->current;
	**l_vars->current = '\0';
	if (ft_strchr(l_vars->token, '$') != NULL)
	{
		handle_with_current_dollar(l_vars, vars, env, node);
		if (vars->error)
			return ;
	}
	else
	{
		*l_vars->result = ft_strjoin(*l_vars->result,
				l_vars->token);
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

void	handle_no_quotes(t_handle_vars *l_vars, t_exec_vars *vars,
				t_env **env, t_parse_tree **node)
{
	l_vars->token = *l_vars->current;
	*l_vars->current = ft_strpbrk(*l_vars->current,
		l_vars->delimiters);
	if (*l_vars->current == NULL)
	{
		handle_no_current(l_vars, vars, env, node);
		return ;
	}
	handle_with_current(l_vars, vars, env, node);
}

/* void handle_quotes_glob_2(t_parse_tree **node, t_exec_vars *vars, t_env **env)
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
} */