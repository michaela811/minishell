#include "minishell.h"

void	handle_quotes_glob_1(t_p_tree **node, t_exec_vars *vars,
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
			return (free_handle_vars(&l_vars));
		}
		if (vars->error)
			break ;
	}
	if (!vars->error)
		return (handle_quotes_final_assign(&vars->args[vars->i],
				l_vars.result, vars));
	free_handle_vars(&l_vars);
}

void	handle_no_current(t_handle_vars *l_vars, t_exec_vars *vars,
		t_env **env, t_p_tree **node)
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
			t_exec_vars *vars, t_env **env, t_p_tree **node)
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
		free(*l_vars->result);
		*l_vars->result = ft_strdup(vars->args[vars->i]);
	}
}

void	handle_with_current(t_handle_vars *l_vars,
		t_exec_vars *vars, t_env **env, t_p_tree **node)
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
				t_env **env, t_p_tree **node)
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
