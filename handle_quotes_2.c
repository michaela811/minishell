/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:37:06 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/24 15:34:50 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_no_current(t_handle_vars *l_vars, t_exec_vars *vars,
		t_env **env, t_p_tree **node)
{
	if (ft_strchr(l_vars->token, '$') != NULL)
	{
		ft_memset(l_vars->buffer, '\0', sizeof(l_vars->buffer));
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
				return ; //(free(*l_vars->result));
		}
	}
	else
		update_args(vars, l_vars);
	if (vars->error)
		return ;//(free(*l_vars->result));
	vars->end = 1;
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

void	handle_error_and_free(t_exec_vars *vars, t_handle_vars *l_vars)
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

void	handle_quotes_final_assign(char **str1, char **str2, t_exec_vars *vars)
{
	*str1 = ft_strdup(*str2);
	check_null(*str1, &vars->error);
	//free_and_null(str2);
}
