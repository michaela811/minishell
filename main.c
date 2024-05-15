#include "lexer.h"

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
    t_free_data     *free_data;

    (void)argc;
    (void)argv;
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, handle_signal);
    free_data = init_command_data(envp);
	while ((input = readline("my(s)hell> ")))
	{
		if (!input)
			break ;
		handle_input(input, free_data);
	}
	rl_on_new_line();
	return (0);
}

void	handle_input(char *input, t_free_data *free_data)
{
    if (*input)
    {
        add_history(input);
        handle_preprocess_input(input, &free_data->token_list);
        if (!free_data->token_list)
            return ;
        handle_parse_tree(&free_data->token_list, &free_data->env);
    }
}

void	handle_preprocess_input(char *input, t_token_list **token_list)
{
	char	*processed_input;

	processed_input = preprocess_input(input, " |><");
	if (processed_input == NULL)
	{
		g_last_exit_status = 2;
		perror("preprocess_input failed");
		free(input);
		input = NULL;
		return ;
	}
	if (lexer(processed_input, token_list))
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

void	handle_parse_tree(t_token_list **token_list, t_env **envmt)
{
	t_parse_tree	*root;
	t_token_list	*start;

	start = *token_list;;
	root = NULL;
	if (is_pipe_sequence(token_list, &root) == SUBTREE_OK)
	{
		if (execute_parse_tree(root, envmt, start))
			g_last_exit_status = PARSING_ERROR;
	}
	else
	{
		g_last_exit_status = 4;
		printf("Parser returned an error: %d\n", SYNTAX_ERROR);
	}
	free_token_list(start);
    free_parse_tree(root);
    check_for_memory_leaks();
}