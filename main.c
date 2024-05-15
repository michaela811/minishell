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
	t_env	        *envmt;
    t_token_list    *token_list;

    token_list = NULL;
    (void)argc;
    (void)argv;
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, handle_signal);
	envmt = init_environment(envp);
	if (envmt == NULL)
	{
		g_last_exit_status = 1;
		return (perror("init_environment failed"), 1);
	}
	while ((input = readline("my(s)hell> ")))
	{
		if (!input)// NOT SURE IF CORRECTLY RESOLVED PREVIOUS while ((input = readline("my(s)hell> ")) != NULL)
			break ;
		handle_input(input, envmt, &token_list);
	}
	rl_on_new_line();
    free_env(envmt);
	return (0);
}

void	handle_input(char *input, t_env *envmt, t_token_list **token_list)
{
	if (*input)
	{
		add_history(input);
		handle_preprocess_input(input, token_list);
		if (!*token_list)
			return ;
		handle_parse_tree(token_list, &envmt);
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
	//t_token_list	*start;
	t_parse_tree	*root;
	t_token_list	*start;

	//start = *token_list;
	start = *token_list;;
	root = NULL;
	if (is_pipe_sequence(token_list, &root) == SUBTREE_OK)
	{
		//g_last_exit_status = PARSING_ERROR;//WHY PARSING ERROR?
		if (execute_parse_tree(root, envmt, start))
		{
			g_last_exit_status = PARSING_ERROR;
			//free_parse_tree(root);
		}
	}
	else
	{
		g_last_exit_status = 4;
        //free_token_list(start);
        //free_parse_tree(root);
		printf("Parser returned an error: %d\n", SYNTAX_ERROR);
	}
	free_token_list(start);
    free_parse_tree(root);
    check_for_memory_leaks();
}