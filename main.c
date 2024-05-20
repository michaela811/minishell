#include "minishell.h"

int	g_last_exit_status = 0;

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
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, handle_signal);
    exec_data = init_command_data(envp);
	while ((input = readline("my(s)hell> ")))
	{
		if (!input)
			break ;
		handle_input(input, exec_data);
		//free_command_data(exec_data);
	}
	rl_on_new_line();
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
		//free_command_data(exec_data);
    }
}

void	handle_preprocess_input(char *input, t_free_data *exec_data)
{
	char	*processed_input;

	processed_input = preprocess_input(input, " |><");
	if (processed_input == NULL)
	{
		//g_last_exit_status = 2;
		//perror("preprocess_input failed");
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
	//else
		//printf("Parser returned an error: %d\n", g_last_exit_status);
	return ;
}