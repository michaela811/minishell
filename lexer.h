#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

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

// Return values
#define SYNTAX_ERROR -1
#define SUBTREE_OK 0
#define MEMORY_ERROR 1

// Allocs and frees
t_parse_tree* alloc_parse_tree();
void free_parse_tree(t_parse_tree *tree);
void free_token(t_token* tok);
void free_token_list(t_token_list* list);

// Errors
void handle_memory_error(t_token **token_list, int num_tokens);

// Parsing
void link_node(t_parse_tree **current, t_parse_tree *newNode);
int is_cmd_word(t_token_list **tok, t_parse_tree **cmd_word_node);
int is_simple_command(t_token_list **tok, t_parse_tree **new);
int is_io_redirect(t_token_list **tok, t_parse_tree **redirect_node);
bool is_redirection_token(enum token_type type);
int is_cmd_prefix(t_token_list **tok, t_parse_tree **prefix_node);
int is_cmd_suffix(t_token_list **tok, t_parse_tree **suffix_node);
void link_pipe(t_parse_tree **current, t_parse_tree *newNode);
int is_pipe_sequence(t_token_list **tok, t_parse_tree **new);

// Printing
void print_parse_tree(t_parse_tree* tree, int depth);

// Testing
void test_parser(); // to delete later

// Tokens
t_token_list* create_token(enum token_type type, char* lexeme); //probably to delete later
void add_token(t_token_list** list, enum token_type type, char* lexeme); // same as above

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

// Execute
void execute_parse_tree(t_parse_tree *root);