#include "lexer.h"

int count_additional_chars(char *input, const char *delim)
{
    int num_of_extra = 0;
    while (*input)
    {
        if (strchr(delim, *input) != NULL)
        {
            if ((*input == '<' && *(input + 1) == '<') || (*input == '>' && *(input + 1) == '>'))
                input++;
            num_of_extra += 2;
        }
        input++;
    }
    return (num_of_extra); 
}

char *preprocess_input(char *str, const char *delim)
{   
    int original_len = strlen(str);
    int additional_chars = count_additional_chars(str, delim);
    // Allocate memory for the preprocessed string
    char *preprocessed = malloc(original_len + additional_chars + 1);
    if (preprocessed == NULL)
    {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    char *dest = preprocessed;

    // Copy the characters from the original string to the preprocessed string
    while (*str != '\0')
    {
        // If the current character is a delimiter, insert a distinct character before it
        if (strchr(delim, *str) != NULL)
        {
            if ((*str == '<' && *(str + 1) == '<') || (*str == '>' && *(str + 1) == '>'))
            {
                *dest++ = '@';
                *dest++ = *str;
                *dest++ = *++str;
                *dest++ = '@';
            }
            else
            {
                *dest++ = '@'; // Insert a distinct character
                *dest++ = *str; // Copy the delimiter
                *dest++ = '@';
            }
        }
        else
            *dest++ = *str; // Copy the character as is
        str++;
    }

    *dest = '\0'; // Null-terminate the preprocessed string
    printf("Preprocessed input is: %s\n", preprocessed);
    return preprocessed;
}

void lexer(char *input, t_token_list **tokenList, int *numTokens)
{
    // Initialize variables
    *numTokens = 0;
    *tokenList = NULL;
    t_token_list *current = NULL;
    //char *delimiter = find_delimiter(input);
    char *tokenValue = strtok(input, " @"); // Tokenize input string based on whitespace

    // Iterate over tokens
    while (tokenValue != NULL)
    {
        // Allocate memory for new token
        t_token *newToken = malloc(sizeof(t_token));
        if (newToken == NULL) {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }

        // Determine token type
        enum token_type tokenType;
        switch (tokenValue[0])
        {
            case '|':
                tokenType = PIPE;
                break;
            case '>':
                tokenType = (tokenValue[1] == '>') ? APPEND : RED_TO;
                break;
            case '<':
                tokenType = (tokenValue[1] == '<') ? HERE_DOC : RED_FROM;
                break;
            default:
                tokenType = WORD;
                break;
        }

        // Populate token list
        newToken -> type = tokenType;
        newToken -> lexeme = strdup(tokenValue);
        if (newToken -> lexeme == NULL) 
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }

        // Move to next token
        t_token_list *newNode = malloc(sizeof(t_token_list));
        if (newNode == NULL)
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
        newNode->token = newToken;
        newNode->next = NULL;

        // Link the new node to the token list
        if (*tokenList == NULL)
        {
            *tokenList = newNode;
            current = *tokenList;
        }
        else
        {
            current->next = newNode;
            current = current->next;
        }

        // Move to the next token
        (*numTokens)++;
        tokenValue = strtok(NULL, " @");
    }
}
