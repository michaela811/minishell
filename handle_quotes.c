/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:37:06 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/05 20:04:49 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int update_result(char **result, char *updated_result, t_exec_vars *vars)
{
    char *new_result;

	new_result = ft_strjoin(*result, updated_result);
	//new_result = NULL;
    if (!check_null(new_result, &vars->error))
        {return (1);}
	//new_result = ft_strjoin(*result, updated_result);
    free(*result); // Free the old string
    *result = new_result; // Assign the new string
	ft_memset(updated_result, '\0', ft_strlen(updated_result));
	return (0);
}

/* void update_result(char **result, char *updated_result, t_exec_vars *vars)
{
    char *new_result;

	new_result = ft_strjoin(*result, updated_result);
	//new_result = NULL;
    if (!check_null(new_result, &vars->error))
        return ;
	//new_result = ft_strjoin(*result, updated_result);
    free(*result); // Free the old string
    *result = new_result; // Assign the new string
} */

void handle_error_and_free(t_exec_vars *vars, t_handle_vars *l_vars)
{
    if (!vars->error)
	{
        free(vars->args[vars->i]);
        vars->args[vars->i] = NULL;
        handle_quotes_final_assign(&vars->args[vars->i], l_vars->result, vars);
        free_handle_vars(l_vars);
		return ;
    }
	//free_env_array(vars->args);
	//free(vars);
	free_handle_vars(l_vars);
}

void handle_error_and_free_redirect(t_exec_vars *vars, t_handle_vars *l_vars, t_p_tree **node)
{
	if (!vars->error)
	{
		free((*node)->child->data->lexeme);
		(*node)->child->data->lexeme = NULL;
		handle_quotes_final_assign(&(*node)->child->data->lexeme, l_vars->result, vars);
		free_handle_vars(l_vars);
		return ;
	}
	//free_env_array(vars->args);
	//free(vars);
	free_handle_vars(l_vars);
}

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
			*l_vars.current = l_vars.current_start;
			return (free_handle_vars(&l_vars));
		}
		if (vars->error)
			break ;
	}
	*l_vars.current = l_vars.current_start;
	handle_error_and_free(vars, &l_vars);
}

void update_args(t_exec_vars *vars, t_handle_vars *l_vars)
{
    free(vars->args[vars->i]);
    vars->args[vars->i] = ft_strjoin(*l_vars->result, l_vars->token);
    if (!check_null(vars->args[vars->i], &vars->error))
        return;
}

int handle_dollar_error(char **token, char *buffer, t_exec_vars *vars,
		 t_env **env)
{
    if(handle_dollar_sign(token, buffer, env, sizeof(buffer)))
    //if (g_last_exit_status)
	{
        vars->error = 1;
        return 1;
    }
    return 0;
}

void	handle_no_current(t_handle_vars *l_vars, t_exec_vars *vars,
		t_env **env, t_p_tree **node)
{
	if (ft_strchr(l_vars->token, '$') != NULL)
	{
		//ft_memset(l_vars->buffer, '\0', sizeof(l_vars->buffer));
		if (handle_dollar_error(&l_vars->token, l_vars->buffer, vars, env))
			return ;
		if (update_result(l_vars->result, l_vars->buffer, vars))
			return ;
		/* update_result(l_vars->result, l_vars->buffer, vars);
		if (vars->error)
			return ; */
		if (strchr((*node)->data->lexeme, '$') != NULL
			&& strchr(l_vars->buffer, ' '))
		{
			vars->i = split_variable(*l_vars->result, vars->i, vars);
			//print_args(vars->args);
			if (vars->error)
			return ;
			//if (remove_spaces(l_vars->result, l_vars->buffer, vars))
				//return ; //(free(*l_vars->result))
			//vars->end = 1;
			//return ;
		}
		else //if (update_result(l_vars->result, l_vars->buffer, vars))
			//return ; //(free(*l_vars->result))
		{
        	free(vars->args[vars->i]);
        	vars->args[vars->i] = ft_strdup(*l_vars->result);
			if (!check_null(vars->args[vars->i], &vars->error))
				return ;//(free(*l_vars->result));
    	}
	}
	else
		update_args(vars, l_vars);
	if (vars->error)
		return ;//(free(*l_vars->result));
	vars->end = 1;
}

int	remove_spaces(char **result, char *buffer, t_exec_vars *vars)
{
	char	*buffer_no_spaces;
	int		j;
	int		k;

	j = 0;
	k = 0;
	buffer_no_spaces = malloc(sizeof(char) * (ft_strlen(buffer) + 1));
	if (!check_null(buffer_no_spaces, &vars->error))
		return (1);
	while (buffer[j])
	{
		if (buffer[j] == ' ')
		{
			buffer_no_spaces[k++] = buffer[j++];
			while (buffer[j] == ' ')
				j++;
		}
		if (buffer[j] != '\0')
			buffer_no_spaces[k++] = buffer[j++];
	}
	buffer_no_spaces[k] = '\0';
	//free(buffer);
	//buffer = buffer_no_spaces;
	if (update_result(result, buffer_no_spaces, vars))
		return (free(buffer_no_spaces), 1);
	return (free(buffer_no_spaces), 0);
}

int	buffer_end_space(char *buffer)
{
	int	i;

	i = ft_strlen(buffer) - 1;
	if (buffer[i] == ' ')
		return (1);
	return (0);
}

void	handle_with_current_dollar(t_handle_vars *l_vars,
			t_exec_vars *vars, t_env **env, t_p_tree **node)
{
	if (handle_dollar_error(&l_vars->token, l_vars->buffer, vars, env))
			return ;
	/* handle_dollar_sign(&l_vars->token, l_vars->buffer, env,
		sizeof(l_vars->buffer));
	if (g_last_exit_status)
	{
		vars->error = 1;
		return ;
	} */
	update_result(l_vars->result, l_vars->buffer, vars);
	if (vars->error)
		return ;
	if (ft_strchr((*node)->data->lexeme, '$') != NULL
		&& ft_strchr(l_vars->buffer, ' '))
	{
		vars->i = split_variable(*l_vars->result, vars->i, vars);
		//if (remove_spaces(l_vars->result, l_vars->buffer, vars))
			//return ; //(free(*l_vars->result))
		if (vars->error)
			return (free(*l_vars->result));//still need it?
		free(*l_vars->result);
		if (buffer_end_space(l_vars->buffer))
		{
			*l_vars->result = ft_strjoin(vars->args[vars->i], " ");
			if (!check_null(*l_vars->result, &vars->error))
				return ;
		}
		else
		{
			*l_vars->result = ft_strdup(vars->args[vars->i]);
			if (!check_null(*l_vars->result, &vars->error))
				return ;
		}
		//*l_vars->result = ft_strdup(vars->args[vars->i]);
		return ;
	}
	//if (update_result(l_vars->result, l_vars->buffer, vars))
		//return ;
}

void	handle_with_current(t_handle_vars *l_vars,
		t_exec_vars *vars, t_env **env, t_p_tree **node)
{
	char	delimiter;
	//char	*new_result;

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
		if (update_result(l_vars->result, l_vars->token, vars))
			return ;
		/* new_result = ft_strjoin(*l_vars->result, l_vars->token);
    	if (!check_null(new_result, &vars->error))
        	return ;
    	free(*l_vars->result); // Free the old string
    	*l_vars->result = new_result; // Assign the new string */
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
	char *temp;

	l_vars->token = *l_vars->current;
	temp = ft_strpbrk(*l_vars->current, l_vars->delimiters);
	if (temp == NULL)
	{
		handle_no_current(l_vars, vars, env, node);
		return ;
	}
	*l_vars->current = temp;
	handle_with_current(l_vars, vars, env, node);
}
