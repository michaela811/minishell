#include "lexer.h"
/*
int lexer(char *input, t_token_list **token_list)
{
    int error = 0;
    *token_list = NULL;
    t_token_list *current = NULL;
    char *token_value = ft_strtok(input, " @", &error);
    if (error)
        return (perror("Memory allocation error"), 1);
    while (token_value != NULL)
    {
        t_token *newToken = malloc(sizeof(t_token));
        if (newToken == NULL)
            return (free_token_list(*token_list), perror("Memory allocation error"), 1);
        newToken->type = determine_token_type(token_value);
        newToken->lexeme = ft_strdup(token_value);
        if (newToken->lexeme == NULL)
            return (free_token(newToken), free_token_list(*token_list), perror("Memory allocation error"), 1);
        t_token_list *newNode = malloc(sizeof(t_token_list));
        if (newNode == NULL)
            return (free_token(newToken), free_token_list(*token_list), perror("Memory allocation error"), 1);
        newNode->token = newToken;
        newNode->next = NULL;
        if (*token_list == NULL)
        {
            *token_list = newNode;
            current = *token_list;
        }
        else
        {
            current->next = newNode;
            current = current->next;
        }
        token_value = ft_strtok(NULL, " @", &error);
        if (error)
        {
            //free(token_value);
            free_token_list(*token_list);
            return ( perror("Memory allocation error"), 1);
        }
            //return (free_token_list(*token_list), free(token_value), perror("Memory allocation error"), 1);
    }
    return (0);
}*/

int lexer(char *input, t_token_list **token_list)
{
    int error;
    t_token_list *current;
    char *token_value;

	error = 0;
    *token_list = NULL;
    current = NULL;
    token_value = ft_strtok(input, " @", &error);
    if (error)
        return (perror("Memory allocation error"), 1);
    while (token_value != NULL)
    {
        if (create_and_add_token(&token_value, token_list, &current, &error))
            return (1);
    }
    return (0);
}

int create_and_add_token(char **token_value, t_token_list **token_list,
t_token_list **current, int *error)
{
	t_token *newToken;
	t_token_list *newNode;

    newToken = create_token_lexer(*token_value);
    if (newToken == NULL)
    	return (free_token_list(*token_list),
			perror("Memory allocation error"), 1);
    newNode = create_node_lexer(newToken);
    if (newNode == NULL)
    	return (free_token(newToken), free_token_list(*token_list),
			perror("Memory allocation error"), 1);
    if (*token_list == NULL)
    {
        *token_list = newNode;
        *current = *token_list;
    }
    else
    {
        (*current)->next = newNode;
        *current = (*current)->next;
    }
    *token_value = ft_strtok(NULL, " @", error);
    if (*error)
    	return (free_token_list(*token_list),
			perror("Memory allocation error"), 1);
    return (0);
}

t_token *create_token_lexer(char *token_value)
{
    t_token *newToken = malloc(sizeof(t_token));
    if (newToken == NULL)
        return NULL;
    newToken->type = determine_token_type(token_value);
    newToken->lexeme = ft_strdup(token_value);
    if (newToken->lexeme == NULL)
    {
        free(newToken);
        return NULL;
    }
    return (newToken);
}

t_token_list *create_node_lexer(t_token *newToken)
{
    t_token_list *newNode = malloc(sizeof(t_token_list));
    if (newNode == NULL)
    {
        free(newToken);
        return NULL;
    }
    newNode->token = newToken;
    newNode->next = NULL;
    return (newNode);
}
