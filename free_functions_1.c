/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_functions_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:34 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 10:36:36 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	free_exit_data(t_free_data *exec_data)
{
	if (exec_data)
	{
		if (exec_data->token_list_start)
		{
			free_token_list(exec_data->token_list_start);
			exec_data->token_list_start = NULL;
		}
		if (exec_data->tree_start)
		{
			free_parse_tree(exec_data->tree_start);
			exec_data->tree_start = NULL;
		}
		if (exec_data->env)
		{
			free_env(exec_data->env);
			exec_data->env = NULL;
		}
		if (exec_data->environment)
		{
			free_env_array(exec_data->environment);
			exec_data->environment = NULL;
		}
	}
}

void	free_command_data(t_free_data *exec_data)
{
	if (exec_data)
	{
		if (exec_data->token_list_start)
		{
			free_token_list(exec_data->token_list_start);
			exec_data->token_list_start = NULL;
		}
		if (exec_data->tree_start)
		{
			free_parse_tree(exec_data->tree_start);
			exec_data->tree_start = NULL;
		}
		if (exec_data->environment)
		{
			free_env_array(exec_data->environment);
			exec_data->environment = NULL;
		}
		if (exec_data->hd_fd != -1)
			close(exec_data->hd_fd);
	}
}

void	free_handle_vars(t_handle_vars *l_vars)
{
	if (l_vars->result == NULL)
		return ;
	if (l_vars->result != NULL)
	{
		if (*l_vars->result != NULL)
		{
			free(*l_vars->result);
			*l_vars->result = NULL;
		}
		free(l_vars->result);
		l_vars->result = NULL;
	}
	if (l_vars->current != NULL)
	{
		if (*l_vars->current != NULL)
		{
			free(*l_vars->current);
			*l_vars->current = NULL;
		}
		free(l_vars->current);
		l_vars->current = NULL;
	}
}

void	free_env_array(char **env_array)
{
	int	i;

	i = 0;
	while (env_array[i] != NULL)
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
	env_array = NULL;
}

void	free_vars(t_exec_vars *vars)
{
    free_env_array(vars->args);
    if (vars->open_fd_in)
        {close(vars->fd_in);}
	if (vars->fd_in != 0)
		{close(vars->fd_in);}
	if (vars->fd_out != 1)
		{close(vars->fd_out);}
    free(vars);
    vars = NULL;
}

/* void	free_and_null(char **ptr)
{
	if (ptr == NULL)
		return ;
	free(*ptr);
	*ptr = NULL;
	free(ptr);
	ptr = NULL;
}

void	free_and_null_double_pointer(char ***ptr)
{
	size_t	i;

	if (ptr != NULL && *ptr != NULL)
	{
		i = 0;
		while ((*ptr)[i] != NULL)
		{
			free((*ptr)[i]);
			(*ptr)[i] = NULL;
			i++;
		}
		free(*ptr);
		*ptr = NULL;
	}
} */
