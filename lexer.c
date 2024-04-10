#include "lexer.h"

size_t handle_quotes(char *str, int *error)
{
	//char *result;
	size_t len = 0;
	char	*c;

	if (!str)
		str++;
	//result = str;
	c = str;
	if (str)
	{
		while(*str)
		{
			str++;
			len++;
			if (*str == *c)
				break;
		}
		if (*str == *c && (*(str + 1) == ' ' || *(str + 1) == '\0'))//maybe '\n'
			*error = 0;
		else
			*error = 1;
	}
	len++;
	return (len);
}

char	*ft_strtok(char *str, const char *delim, int *error)
{
	static char	*i;
	char		*result;
	size_t len;

	result = str;
	if (str)
		i = str;
	if (i && (*i == '"' || *i == '\''))
	{
		len = handle_quotes(i, error);
		result = i;
		i += len;
		str = i;
		i = update_pointer(str);
		return (result);
	}
	while (i && *i && strchr(delim, *i))
		i++;
	str = i;
	while (str && *str)
	{
		result = str;
		while (str && !strchr(delim, *str))
			str++;
		i = update_pointer(str);
	}
	return (result);
}

char *update_pointer(char *str)
{
    if (str && *str)
	{
        *str = '\0';
        return (str + 1);
	}
    else
        return str;
}

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

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
    //printf("Preprocessed input is: %s\n", preprocessed);
    return preprocessed;
}

enum token_type determine_token_type(char *token_value)
{
    if (strcmp(token_value, "|") == 0)
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

/* t_token *allocate_and_copy_tokens(t_token **token_list, int num_tokens)
{
    t_token *new_token_list = malloc((num_tokens + 1) * sizeof(t_token)); // Allocate memory for new token
    if (new_token_list == NULL)
    {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    if (*token_list != NULL)
	{
        memcpy(new_token_list, *token_list, num_tokens * sizeof(t_token)); // Copy old data to new block
        free(*token_list); // Free old block
    }
    return new_token_list;
}*/ // Not needed, we switched to t_token_list from t_token

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
        if (newToken == NULL) {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }

        /* Determine token type
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
        }*/

        // Populate token list
        newToken -> type = determine_token_type(tokenValue);
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
        tokenValue = ft_strtok(NULL, " @", error);
    }
}
