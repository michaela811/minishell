#include "minishell.h"

int			g_last_exit_status = 0;

void	handle_signal(int signal)
{
	const char	*prompt;

	prompt = "\nmy(s)hell> ";
	if (signal == SIGINT)
		write(1, prompt, ft_strlen(prompt));
	else if (signal == SIGQUIT)
	{
	}
}

int	main(int argc, char **argv, char **envp)
{
	char	        *input;
    t_free_data     *exec_data;

    (void)argc;
    (void)argv;
	exec_data = init_command_data(envp);
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, handle_signal);
	while ((input = readline("my(s)hell> ")))
	{
		if (!input)
			break ;
		handle_input(input, exec_data);
		//free_command_data(exec_data);
	}
	/* while (1)
    {
        if (isatty(fileno(stdin)))
            input = readline("my(s)hell> ");
        else
        {
            char *line;
            line = get_next_line(fileno(stdin));
            input = ft_strtrim(line, "\n");
            free(line);
        }
        if (!input)
		{
			free_exit_data(exec_data);
            break ;
		}
        handle_input(input, exec_data);
    } */
	//rl_on_new_line();
	return (0);
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

	processed_input = preprocess_input(input, " |><");
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
	else
		free_command_data(exec_data);
	return ;
}