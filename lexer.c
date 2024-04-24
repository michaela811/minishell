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
    int original_len;
    int additional_chars;
    char *preprocessed;

    original_len = ft_strlen(str);
    additional_chars = count_additional_chars(str, delim);
    preprocessed = malloc(original_len + additional_chars + 1);
    if (preprocessed == NULL)
        return (perror("Memory allocation error"), NULL);
    char *dest = preprocessed;
    while (*str != '\0')
    {
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
                *dest++ = '@';
                *dest++ = *str;
                *dest++ = '@';
            }
        }
        else
            *dest++ = *str;
        str++;
    }
    *dest = '\0';
    return (preprocessed);
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

int lexer(char *input, t_token_list **tokenList)
{
    int error = 0;
    *tokenList = NULL;
    t_token_list *current = NULL;
    char *tokenValue = ft_strtok(input, " @", &error);
    if (error)
        return (perror("Memory allocation error"), 1);
    while (tokenValue != NULL)
    {
        t_token *newToken = malloc(sizeof(t_token));
        if (newToken == NULL)
            return (free_token_list(*tokenList), perror("Memory allocation error"), 1);
        newToken->type = determine_token_type(tokenValue);
        newToken->lexeme = ft_strdup(tokenValue);
        if (newToken->lexeme == NULL)
            return (free_token(newToken), free_token_list(*tokenList), perror("Memory allocation error"), 1);
        t_token_list *newNode = malloc(sizeof(t_token_list));
        if (newNode == NULL)
            return (free_token(newToken), free_token_list(*tokenList), perror("Memory allocation error"), 1);
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
        tokenValue = ft_strtok(NULL, " @", &error);
        if (error)
        {
            //free(tokenValue);
            free_token_list(*tokenList);
            return ( perror("Memory allocation error"), 1);
        }
            //return (free_token_list(*tokenList), free(tokenValue), perror("Memory allocation error"), 1);
    }
    return (0);
}
