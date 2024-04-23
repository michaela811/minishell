#include "lexer.h"

int main(int argc, char **argv, char **envp)
{
    char* input;
	int error = 0;
    t_env *envmt = init_environment(envp);
    if (envmt == NULL)
        return (perror("init_environment failed"), 1);
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
            int numTokens = 0;
            char *processed_input = preprocess_input(input, "|><");
            lexer(processed_input, &tokenList, &numTokens, &error);
            t_token_list *current = tokenList; // Start from the head of the token list
            int i = 1;
            t_parse_tree* root = NULL;
            if (is_pipe_sequence(&tokenList, &root) == SUBTREE_OK)
            {
                /*printf("Parse tree:\n");
                print_parse_tree(root, 0);*/

                if (execute_parse_tree(root, &envmt))
                    free_parse_tree(root);
            }
            else
                printf("Parser returned an error: %d\n", SYNTAX_ERROR);
            free(processed_input);
            free(tokenList);
        }
    }
    rl_on_new_line();
    free_env(envmt);
    free(input);
    return 0;
}