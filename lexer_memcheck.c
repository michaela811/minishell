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
    HERE_DOC,   // 6
    ASSIGNMENT_WORD, // to delete when possible
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


// Lexer helper
int count_additional_chars(char *input, const char *delim);
char *preprocess_input(char *str, const char *delim);
size_t handle_quotes(char *str, int *error);
char	*ft_strtok(char *str, const char *delim, int *error); // Maybe add to Libft
int	ft_strlen(const char *s); // same as above
char *update_pointer(char *str); // same?

// Lexer
void lexer(char *input, t_token_list **tokenList, int *numTokens, int *error);
enum token_type determine_token_type(char *token_value);

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
	while (i && *i && strchr(delim, *i))
		i++;
	if (i && (*i == '"' || *i == '\''))
	{
		len = handle_quotes(i, error);
		result = i;
		i += len;
		str = i;
		i = update_pointer(str);
		return (result);
	}
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

char *allocate_memory_for_preprocessed_input(char *str, const char *delim)
{
    int original_len = strlen(str);
    int additional_chars = count_additional_chars(str, delim);
    char *preprocessed = malloc(original_len + additional_chars + 1);
    if (preprocessed == NULL)
    {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    return preprocessed;
}

void process_character(char **dest, char **str, const char *delim)
{
    if (strchr(delim, **str) != NULL)
    {
        if ((**str == '<' && *(*str + 1) == '<') || (**str == '>' && *(*str + 1) == '>'))
        {
            *(*dest)++ = '@';
            *(*dest)++ = **str;
            *(*dest)++ = *(*str)++;
            *(*dest)++ = '@';
        }
        else
        {
            *(*dest)++ = '@'; // Insert a distinct character
            *(*dest)++ = **str; // Copy the delimiter
            *(*dest)++ = '@';
        }
    }
	else if (**str == '\'' || **str == '"')
		{
			*(*dest)++ = '@';
			*(*dest)++ = **str;
			(*str)++;
			while (**str != '\'' && **str != '"')
				*(*dest)++ = *(*str)++;
			*(*dest)++ = **str;
			*(*dest)++ = '@';
		}
    else
        *(*dest)++ = **str; // Copy the character as is
    (*str)++;
}

char *preprocess_input(char *str, const char *delim)
{
    char *preprocessed = allocate_memory_for_preprocessed_input(str, delim);
    char *dest = preprocessed;
    while (*str != '\0')
    {
        process_character(&dest, &str, delim);
    }
    *dest = '\0'; // Null-terminate the preprocessed string
    printf("Preprocessed input is: %s\n", preprocessed);
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

t_token *create_new_token(char *tokenValue)
{
    t_token *newToken = malloc(sizeof(t_token));
    if (newToken == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    newToken->type = determine_token_type(tokenValue);
    newToken->lexeme = strdup(tokenValue);
    if (newToken->lexeme == NULL)
    {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    return newToken;
}

t_token_list *create_new_node(t_token *newToken)
{
    t_token_list *newNode = malloc(sizeof(t_token_list));
    if (newNode == NULL)
    {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }
    newNode->token = newToken;
    newNode->next = NULL;
    return newNode;
}

void add_node_to_list(t_token_list **tokenList, t_token_list **current, t_token_list *newNode)
{
    if (*tokenList == NULL)
    {
        *tokenList = newNode;
        *current = *tokenList;
    }
    else
    {
        (*current)->next = newNode;
        *current = (*current)->next;
    }
}

void lexer(char *input, t_token_list **tokenList, int *numTokens, int *error)
{
    // Initialize variables
    *numTokens = 0;
    *tokenList = NULL;
    t_token_list *current = NULL;
    char *tokenValue = ft_strtok(input, " @", error); // Tokenize input string based on whitespace

    // Iterate over tokens
    while (tokenValue != NULL)
    {
        t_token *newToken = create_new_token(tokenValue);
        t_token_list *newNode = create_new_node(newToken);
        add_node_to_list(tokenList, &current, newNode);

        // Move to the next token
        (*numTokens)++;
        tokenValue = ft_strtok(NULL, " @", error);
    }
}

void print_token_list(t_token_list *tokenList)
{
    t_token_list *current = tokenList;
    while (current != NULL)
    {
        printf("Type: %d, Lexeme: %s\n", current->token->type, current->token->lexeme);
        current = current->next;
    }
}

int main()
{
    char input[] = "echo 'hello world' | cat >> file.txt";
    t_token_list *tokenList = NULL;
    int numTokens = 0;
    int error = 0;

    char *preprocessed_input = preprocess_input(input, " @><|");
    lexer(preprocessed_input, &tokenList, &numTokens, &error);

	// Print the token list
    print_token_list(tokenList);

    // Free the preprocessed input
    free(preprocessed_input);

    // Free the token list
    t_token_list *current = tokenList;
    while (current != NULL)
    {
        t_token_list *next = current->next;
        free(current->token->lexeme);
        free(current->token);
        free(current);
        current = next;
    }

    return 0;
}
