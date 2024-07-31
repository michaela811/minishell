/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:08 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/24 16:29:47 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_exec_vars(t_exec_vars *vars)
{
	int	i;

	vars->fd_in = 0;
	vars->open_fd_in = 0;
	vars->fd_out = 1;
	vars->capacity = 10;
	vars->args = malloc(vars->capacity * sizeof(char *));
	if (!vars->args)
	{
		vars->error = 1;
		return ;
	}
	i = 0;
	while (i < vars->capacity)
		vars->args[i++] = NULL;
	vars->i = 0;
	vars->error = 0;
	vars->inside_single_quotes = 0;
	vars->inside_double_quotes = 0;
	vars->end = 0;
}

// Custom realloc function
/* void *ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void *new_ptr;
	size_t copy_size;

    // If new_size is zero, free the memory and return NULL
    if (new_size == 0)
		return (free(ptr) , NULL);
    // If ptr is NULL, allocate new memory
    if (ptr == NULL)
        return (malloc(new_size));
    // Allocate new memory
    new_ptr = malloc(new_size);
    if (!new_ptr)
        return (NULL);
    // Copy the old data to the new memory block
    if (old_size < new_size)
        copy_size = old_size;
    else
        copy_size = new_size;
    ft_memcpy(new_ptr, ptr, copy_size);
    // Free the old memory block
    free(ptr);
    // Return the new memory block
    return (new_ptr);
} */


void	expand_exec_vars(t_exec_vars *vars)
{
	int		new_capacity;
	char	**new_args;
	int		i;
	size_t original_size;
	size_t new_size;

	i = vars->capacity;
	new_capacity = vars->capacity * 2;
	original_size = vars->capacity * sizeof(char *);
    new_size = new_capacity * sizeof(char *);
	new_args = ft_realloc(vars->args, original_size, new_size);
	//new_args = realloc(vars->args, new_capacity * sizeof(char *));
	if (new_args == NULL)
	{
		vars->error = 1;
		return ;
	}
	vars->args = new_args;
	while (i < new_capacity)
		vars->args[i++] = NULL;
	vars->capacity = new_capacity;
}

int	split_variable(char *arg, int i, t_exec_vars *vars)
{
	char	**split_args;
	int		j;

	j = 0;
	split_args = ft_split(arg, ' ');
	if (!split_args)
	{
		vars->error = 1;
		return (i);
	}
	free(vars->args[vars->i]);//check later if needed!!!
	while (split_args[j])
	{
		vars->args[i + j] = split_args[j];
		j++;
	}
	free(split_args);
	return (i + (j - 1));
}
