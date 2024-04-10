#include "lexer.h"

int main()
{
    char input[100]; // Adjust size as needed
	int error = 0;

    printf("Enter a command: ");
    fgets(input, sizeof(input), stdin); // Read input from the command line

    // Remove the newline character from the input
    input[strcspn(input, "\n")] = '\0';

    t_token_list *tokenList = NULL;
    int numTokens = 0;
    // Processed the input here, so we can free it atfer using it
    char *processed_input = preprocess_input(input, "|><");
    // Tokenize the input string using the lexer
    lexer(processed_input, &tokenList, &numTokens, &error);

    // Print the token list for verification
    printf("Token list:\n");
    t_token_list *current = tokenList; // Start from the head of the token list
    int i = 1; // Token index

// Traverse the linked list of tokens
    while (current != NULL)
    {
        printf("Token %d: Type=%d, Value='%s'\n", i, current->token->type, current->token->lexeme);
        current = current->next;
        i++;
    }

    // Parse the tokenized tree
    t_parse_tree* root = NULL;
    if (is_pipe_sequence(&tokenList, &root) == SUBTREE_OK)
    {
        printf("Parse tree:\n");
        print_parse_tree(root, 0);
        execute_parse_tree(root);
    }
    else
    {
        //free_parse_tree(root);//MAYBE NOT NEEDED IF WAS FREED IN IS_PIPE_SEQUENCE
        printf("Parser returned an error: %d\n", SYNTAX_ERROR);
    }

    // Free resources
    // free_token_list(tokenList); Fix later


    // Free allocated memory
    /*for (int i = 0; i < numTokens; i++) {
        free(tokenList[i].lexeme);
    }*/

    free(processed_input);
    free(tokenList);

    return 0;
}