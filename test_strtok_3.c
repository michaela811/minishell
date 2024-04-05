#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum token_type
{
    PIPE = 1,   // 1
    WORD,       // 2
    RED_TO,     // 3
    RED_FROM,   // 4
    APPEND,     // 5
    HERE_DOC    // 6
};

typedef struct s_token
{
    char *lexeme;
    enum token_type type;
} t_token;


//from lexer
typedef struct s_token_list
{
    struct s_token *token;
    struct s_token_list *next;
} t_token_list;

typedef struct s_parse_tree
{
  struct s_token *data;
  struct s_parse_tree *child;
  struct s_parse_tree *sibling;
} t_parse_tree;


char *preprocess_input(char *str, const char *delim)
{
    // Allocate memory for the preprocessed string
    char *preprocessed = malloc(strlen(str) * 2 + 1); // Double the size to handle worst-case scenario
    if (preprocessed == NULL)
	{
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    char *dest = preprocessed;
    // Copy the characters from the original string to the preprocessed string
    while (*str != '\0') {
        // If the current character is a delimiter, insert a distinct character before it
        if (strchr(delim, *str) != NULL)
		{
            *dest++ = '@'; // Insert a distinct character
            *dest++ = *str; // Copy the delimiter
            *dest++ = '@';
        }
		else
            *dest++ = *str; // Copy the character as is
        str++;
    }

    *dest = '\0'; // Null-terminate the preprocessed string
    printf("Preprocessed input is: %s\n", preprocessed);
    return preprocessed;
}

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
/*
char	*ft_strtok(char *str, const char *delim)
{
	static char	*i;
	char		*result;

	result = str;
	if (str)
		i = str;
	while (*i && ft_strchr(delim, *i))
		i++;
	str = i;
	while (*str)
	{
		result = str;
		while (str && !ft_strchr(delim, *str))
			str++;
		if (*str)
			i = str + 1;
		else
			i = str;
		*str = '\0';
	}
	return (result);
}
*/

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

t_token *allocate_and_copy_tokens(t_token **token_list, int num_tokens)
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

void lexer(char *input, t_token **token_list, int *num_tokens, int *error)
{
    *num_tokens = 0;
    *token_list = NULL;
    char *processed_input;
    char *token_value;

    processed_input = preprocess_input(input, "|><");
    token_value = ft_strtok(processed_input, " @", error);
	printf("token_value: %s\n", token_value);
	printf("error: %d\n", *error);
    while (token_value != NULL && *error == 0) // Iterate over tokens
    {
        *token_list = allocate_and_copy_tokens(token_list, *num_tokens);

        (*token_list)[*num_tokens].type = determine_token_type(token_value);
        (*token_list)[*num_tokens].lexeme = strdup(token_value);
        if ((*token_list)[*num_tokens].lexeme == NULL)
        {
            handle_memory_error(token_list, *num_tokens);
        }
        (*num_tokens)++; // Move to next token
        token_value = ft_strtok(NULL, " @", error);
    }
}
/*
void lexer(char *input, t_token **token_list, int *num_tokens)
{
    *num_tokens = 0;
    *token_list = NULL;
    char *processed_input;
    char *token_value;
	int *error = 0;

	processed_input = preprocess_input(input, "|><");
    token_value = ft_strtok(processed_input, " @", error);
    while (token_value != NULL && !error) // Iterate over tokens
    {
        *token_list = malloc((*num_tokens + 1) * sizeof(t_token)); // Allocate memory for new token
        if (*token_list == NULL)
        {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
        (*token_list)[*num_tokens].type = determine_token_type(token_value);
        (*token_list)[*num_tokens].lexeme = strdup(token_value);
        if ((*token_list)[*num_tokens].lexeme == NULL)
        {
			free(*token_list);//should be freed in a different way
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
        (*num_tokens)++; // Move to next token
        token_value = ft_strtok(NULL, " @", error);
    }
}
*/

int main() {
    char input[100]; // Adjust size as needed

    printf("Enter a command: ");
    fgets(input, sizeof(input), stdin); // Read input from the command line

    // Remove the newline character from the input
    input[strcspn(input, "\n")] = '\0';

    t_token *token = NULL;
    int num_tokens = 0;

	int error = 0;

    // Tokenize the input string using the lexer
    lexer(input, &token, &num_tokens, &error);

    // Pass the token list to your parser functions for parsing
    // Example: parse_logical_and(token, num_tokens);

    // Print the token list for verification
    printf("Token list:\n");
    for (int i = 0; i < num_tokens; i++) {
        printf("Token %d: Type=%d, Value='%s'\n", i+1, token[i].type, token[i].lexeme);
    }

    // Free allocated memory
    for (int i = 0; i < num_tokens; i++) {
        free(token[i].lexeme);
    }
    free(token);

    return 0;
}