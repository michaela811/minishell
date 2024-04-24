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
                free_env(envmt);
                free(input);
                rl_clear_history();
                exit (0);
            }
            add_history(input);
            t_token_list *tokenList = NULL;
            //int numTokens = 0;
            char *processed_input = preprocess_input(input, "|><");
            if (processed_input == NULL)
            {
                perror("preprocess_input failed");
                free(input);
                input = NULL;
                continue;
            }
            if (lexer(processed_input, &tokenList))//, &numTokens))
            {
                //perror("lexer failed");
                free(input);
                input = NULL;
                free(processed_input);
                processed_input = NULL;
                continue;
            }

            t_token_list *start = tokenList; // Start from the head of the token list
            int i = 1;
            t_parse_tree* root = NULL;
            if (is_pipe_sequence(&tokenList, &root) == SUBTREE_OK)
            {
                execute_parse_tree(root, &envmt);
                free_parse_tree(root);
            }
            else
            {
                printf("Parser returned an error: %d\n", SYNTAX_ERROR);
                //free_token_list(current);
            }
            free_token_list(start);
            free(input);
            free(processed_input);
        }
    }
    rl_on_new_line();
    free_env(envmt);
    //free(input);maybe not needed?
    return 0;
}


/*
int main() {
    char input[] = "This is a test";
    t_token_list *tokenList = NULL;
    int error = lexer(input, &tokenList);
    if (error) {
        printf("Lexer returned an error\n");
    } else {
        printf("Lexer returned successfully\n");
        t_token_list *current = tokenList;
        while (current != NULL) {
            printf("Token type: %d, lexeme: %s\n", current->token->type, current->token->lexeme);
            current = current->next;
        }
    }
    free_token_list(tokenList);
    return 0;
}
*/
/*
int main()
{
    test_parser();
    return 0;
}*/
