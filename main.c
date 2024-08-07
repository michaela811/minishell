/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:37:43 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/07 18:21:34 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				g_last_exit_status = 0;

int	main(int argc, char **argv, char **envp)
{
	t_free_data			exec_data;

	(void)argc;
	(void)argv;
	init_command_data(envp, &exec_data);
	setup_signal_handlers();
	while (1)
	{
		if (process_input(&exec_data))
			break ;
	}
	return (exec_data.dollar_status);
}

int	process_input(t_free_data *exec_data)
{
	char	*input;

	exec_data->dollar_status = g_last_exit_status;
	input = readline("my(s)hell> ");
	if (!input)
	{
		free_exit_data(exec_data);
		clear_history();
		return (1);
	}
	handle_input(input, exec_data);
	return (0);
}
