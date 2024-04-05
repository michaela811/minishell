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
    HERE_DOC,    // 6
    ASSIGNMENT_WORD
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

// Parsing
t_parse_tree* alloc_parse_tree();
void free_parse_tree(t_parse_tree *tree);
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
void test_parser();

// Tokens
void free_token(t_token* tok);
t_token_list* create_token(enum token_type type, char* lexeme); //see if it is necessary to include
void add_token(t_token_list** list, enum token_type type, char* lexeme); // same as above
void free_token_list(t_token_list* list);

// Lexer helper
int count_additional_chars(char *input, const char *delim);
char *preprocess_input(char *str, const char *delim);


// Lexer
void lexer(char *input, t_token_list **tokenList, int *numTokens);