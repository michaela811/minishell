/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_handle_input.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 14:24:35 by dpadenko          #+#    #+#             */
/*   Updated: 2024/08/06 14:25:28 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_input(char *input, t_free_data *exec_data)
{
	if (*input)
	{
		add_history(input);
		handle_preprocess_input(input, exec_data);
		if (!exec_data->token_list)
			return ;
		exec_data->token_list_start = exec_data->token_list;
		handle_parse_tree(exec_data);
	}
}

void	handle_preprocess_input(char *input, t_free_data *exec_data)
{
	char	*processed_input;

	processed_input = preprocess_input(input, " |><\t");
	if (processed_input == NULL)
	{
		free(input);
		input = NULL;
		return ;
	}
	if (lexer(processed_input, &(exec_data->token_list)))
	{
		g_last_exit_status = 3;
		free(input);
		input = NULL;
		free(processed_input);
		processed_input = NULL;
		return ;
	}
	free(input);
	free(processed_input);
}

void	handle_parse_tree(t_free_data *exec_data)
{
	if (is_pipe_sequence(exec_data) == 0)
	{
		execute_parse_tree(exec_data);
		free_command_data(exec_data);
	}
	return ;
}
