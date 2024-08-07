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

int	main(int argc, char **argv, char **envp)
{
	t_free_data			exec_data;
	struct termios		orig_termios;

	(void)argc;
	(void)argv;
	init_command_data(envp, &exec_data);
	setup_signal_handlers();
	if (isatty(STDIN_FILENO))
	{
		if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
		{
			print_err(errno, 2, "tcgetattr");
			exit(EXIT_FAILURE);
		}
	}
	while (1)
	{
		if (process_input(&exec_data, &orig_termios))
			break ;
	}
	return (exec_data.dollar_status);
}

int	process_input(t_free_data *exec_data, struct termios *orig_termios)
{
	char	*input;

	exec_data->dollar_status = g_last_exit_status;
	g_last_exit_status = 0;
	set_raw_mode(orig_termios);
	input = readline("my(s)hell> ");
	if (!input)
	{
		if (isatty(fileno(stdin)))
			reset_terminal_mode(orig_termios);
		free_exit_data(exec_data);
		clear_history();
		return (1);
	}
	reset_terminal_mode(orig_termios);
	handle_input(input, exec_data);
	return (0);
}
