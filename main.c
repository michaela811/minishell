/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:37:43 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 10:37:45 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				g_last_exit_status = 0;
struct termios	orig_termios;

void	reset_terminal_mode(void)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

void	set_raw_mode(void)
{
	struct termios	raw;

	tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(reset_terminal_mode);
	raw = orig_termios;
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
	raw.c_cflag |= (CS8);
	raw.c_oflag &= ~(OPOST);
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 0;
	raw.c_lflag |= ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void	handle_signal(int signal)
{
	if (signal == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (signal == SIGQUIT)
	{
	}
}

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_free_data	*exec_data;

	(void)argc;
	(void)argv;
	exec_data = init_command_data(envp);
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	/*while (1)
	{
		set_raw_mode();
		input = readline("my(s)hell> ");
		reset_terminal_mode();
		if (!input)
		{
			printf("exit\n");
			free_exit_data(exec_data);
			break;
		}
		handle_input(input, exec_data);
	}*/
	while (1)
	{
		if (isatty(fileno(stdin)))
		{
			set_raw_mode();
			input = readline("my(s)hell> ");
			reset_terminal_mode();
		}
		else
		{
			char	*line;
			line = get_next_line(fileno(stdin));
			input = ft_strtrim(line, "\n");
			free(line);
		}
		if (!input)
		{
			free_exit_data(exec_data);
			clear_history();
			break ;
		}
		handle_input(input, exec_data);
	}
	return (g_last_exit_status);
}

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
		//check_for_memory_leaks();
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
