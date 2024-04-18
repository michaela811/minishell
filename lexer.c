#include "lexer.h"

int count_additional_chars(char *input, const char *delim)
{
    int num_of_extra = 0;
    while (*input)
    {
        if (ft_strchr(delim, *input) != NULL)
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
    int original_len = ft_strlen(str);
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
        if (ft_strchr(delim, *str) != NULL)
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
    //printf("Preprocessed input is: %s\n", preprocessed);
    return preprocessed;
}

enum token_type determine_token_type(char *token_value)
{
    if (strcmp(token_value, "|") == 0) // ad this in libft library
        return PIPE;
    else if (strcmp(token_value, ">") == 0)
        return RED_TO;
    else if (strcmp(token_value, "<") == 0)
        return RED_FROM;
    else if (strcmp(token_value, ">>") == 0)
        return APPEND;
    else if (strcmp(token_value, "<<") == 0)
        return HERE_DOC;
    else
        return WORD;
}

void handle_memory_error(t_token **token_list, int num_tokens)
{
	int i = 0;

	while (i < num_tokens)
	{
		free((*token_list)[i].lexeme);
		i++;
	}
    free(*token_list);
    perror("Memory allocation error");
    exit(EXIT_FAILURE);
}

void lexer(char *input, t_token_list **tokenList, int *numTokens, int *error)
{
    // Initialize variables
    *numTokens = 0;
    *tokenList = NULL;
    t_token_list *current = NULL;
    //char *delimiter = find_delimiter(input);
    char *tokenValue = ft_strtok(input, " @", error); // Tokenize input string based on whitespace

    // Iterate over tokens
    while (tokenValue != NULL)
    {
        // Allocate memory for new token
        t_token *newToken = malloc(sizeof(t_token));
        if (newToken == NULL)
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
        newToken -> type = determine_token_type(tokenValue);
        newToken -> lexeme = ft_strdup(tokenValue);
        if (newToken -> lexeme == NULL)
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
        t_token_list *newNode = malloc(sizeof(t_token_list));
        if (newNode == NULL)
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
        newNode->token = newToken;
        newNode->next = NULL;
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
        (*numTokens)++;
        tokenValue = ft_strtok(NULL, " @", error);
    }
}
