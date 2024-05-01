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
	char	*input;
	t_env	*envmt;

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
		handle_input(input, envmt);
	}
	rl_on_new_line();
	free_env(envmt);
	return (0);
}

void	handle_input(char *input, t_env *envmt)
{
	t_token_list	*token_list;

	if (*input)
	{
		if (strcmp(input, "exit") == 0)
		{
			free_env(envmt);
			free(input);
			rl_clear_history();
			exit (0);
		}
		add_history(input);
		token_list = NULL;
		handle_preprocess_input(input, &token_list);
		if (!token_list)
			return ;
		handle_parse_tree(&token_list, &envmt);
	}
}

void	handle_preprocess_input(char *input, t_token_list **token_list)
{
	char	*processed_input;

	processed_input = preprocess_input(input, "|><");
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

	//start = *token_list;
	root = NULL;
	if (is_pipe_sequence(token_list, &root) == SUBTREE_OK)
	{
		//g_last_exit_status = PARSING_ERROR;//WHY PARSING ERROR?
		if (execute_parse_tree(root, envmt))
		{
			g_last_exit_status = PARSING_ERROR;
			free_parse_tree(root);
		}
	}
	else
	{
		g_last_exit_status = 4;
		printf("Parser returned an error: %d\n", SYNTAX_ERROR);
	}
}

/*
int main(int argc, char **argv, char **envp)
{
    char* input;
	int error = 0;
    t_env *envmt;

    signal(SIGINT, handle_signal);
    signal(SIGQUIT, handle_signal);
    envmt = init_environment(envp);
    if (envmt == NULL)
    {
        g_last_exit_status = 1;
        return (perror("init_environment failed"), 1);
    }
    while ((input = readline("my(s)hell> ")) != NULL)
    {
        if (*input)
        {
            if (strcmp(input, "exit") == 0)
            {
                free_env(envmt);
                free(input);
                rl_clear_history();
                exit (0);
            }
            add_history(input);
            t_token_list *token_list = NULL;
            //int numTokens = 0;
            char *processed_input = preprocess_input(input, "|><");
            if (processed_input == NULL)
            {
                g_last_exit_status = 2;
                perror("preprocess_input failed");
                free(input);
                input = NULL;
                continue;
            }
            if (lexer(processed_input, &token_list))//, &numTokens))
            {
                //perror("lexer failed");
                g_last_exit_status = 3;
                free(input);
                input = NULL;
                free(processed_input);
                processed_input = NULL;
                continue;
            }

            t_token_list *start = token_list; // Start from the head of the token list
            int i = 1;
            t_parse_tree* root = NULL;
            if (is_pipe_sequence(&token_list, &root) == SUBTREE_OK)
            {
                g_last_exit_status = PARSING_ERROR;
                if (execute_parse_tree(root, &envmt))
                {
                    g_last_exit_status = PARSING_ERROR;
                    free_parse_tree(root);
                }
            }
            else
            {
                g_last_exit_status = 4;
                printf("Parser returned an error: %d\n", SYNTAX_ERROR);
            }
            free(processed_input);
        }
    }
    rl_on_new_line();
    free_env(envmt);
    //free(input);maybe not needed?
    return 0;
}
*/


/*
int main() {
    char input[] = "This is a test";
    t_token_list *token_list = NULL;
    int error = lexer(input, &token_list);
    if (error) {
        printf("Lexer returned an error\n");
    } else {
        printf("Lexer returned successfully\n");
        t_token_list *current = token_list;
        while (current != NULL) {
            printf("Token type: %d, lexeme: %s\n", current->token->type, current->token->lexeme);
            current = current->next;
        }
    }
    free_token_list(token_list);
    return 0;
}
*/
/*
int main()
{
    test_parser();
    return 0;
}*/
