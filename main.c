#include "lexer.h"

void handle_signal(int signal)
{
    const char *prompt;

    prompt = "\nmy(s)hell> ";
    if (signal == SIGINT)
        write(1, prompt, ft_strlen(prompt));
    else if (signal == SIGQUIT)
    {
    }
}

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
                free(input);
                exit (0);
            }
            add_history(input);
            t_token_list *tokenList = NULL;
            //int numTokens = 0;
            char *processed_input = preprocess_input(input, "|><");
            if (processed_input == NULL)
            {
                g_last_exit_status = 2;
                perror("preprocess_input failed");
                free(input);
                continue;
            }
            if (lexer(processed_input, &tokenList))//, &numTokens))
            {
                //perror("lexer failed");
                g_last_exit_status = 3;
                free(input);
                free(processed_input);
                continue;
            }
            t_token_list *current = tokenList; // Start from the head of the token list
            int i = 1;
            t_parse_tree* root = NULL;
            if (is_pipe_sequence(&tokenList, &root) == SUBTREE_OK)
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
            free_token_list(tokenList);
        }
    }
    rl_on_new_line();
    free_env(envmt);
    free(input);
    return 0;
}