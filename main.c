/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:37:43 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/01 11:32:52 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				g_last_exit_status = 0;

void	reset_terminal_mode(struct termios *orig_termios)
{
	tcsetattr(STDIN_FILENO, TCSANOW, orig_termios);
}

void	set_raw_mode(struct termios *orig_termios)
{
	struct termios	raw;

	tcgetattr(STDIN_FILENO, orig_termios);
	raw = *orig_termios;
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
	int	saved_errno;

	if (signal == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_last_exit_status = 130;
	}
	else if (signal == SIGQUIT)
	{
	}
	else if (signal == SIGCHLD || signal == SIGPIPE)
	{
		saved_errno = errno;
		while (waitpid((pid_t)(-1), 0, WNOHANG) > 0)
			continue ;
		errno = saved_errno;
	}
}

void	setup_signal_handlers(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
	if (sigaction(SIGCHLD, &sa, NULL) == -1)
	{
		print_err(errno, 2, "my(s)hell: sigaction sigchild");
		exit(EXIT_FAILURE);
	}
	if (sigaction(SIGPIPE, &sa, NULL) == -1)
    {
        print_err(errno, 2, "my(s)hell: sigaction sigpipe");
        exit(EXIT_FAILURE);
    }
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
}

int	main(int argc, char **argv, char **envp)
{
	char				*input;
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
	/*while (1)
	{
		set_raw_mode(&orig_termios);
		input = readline("my(s)hell> ");
		reset_terminal_mode(&orig_termios);
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
			set_raw_mode(&orig_termios);
			input = readline("my(s)hell> ");
			reset_terminal_mode(&orig_termios);
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
			if (isatty(fileno(stdin)))
				reset_terminal_mode(&orig_termios);
			free_exit_data(&exec_data);
			clear_history();
			break ;
		}
		handle_input(input, &exec_data);
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
