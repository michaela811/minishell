#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"

enum token_type
{
    PIPE = 1,   // 1
    WORD,       // 2
    RED_TO,     // 3 >
    RED_FROM,   // 4 <
    APPEND,     // 5 >>
    HERE_DOC,   // 6 <<
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

typedef struct s_env
{
    char *name;
    char *value;
    struct s_env *next;
} t_env;

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
void	error_message(char *str); // to delete later
void	free_array(char **array); // to delete later
void	exit_function(int i); // to delete later
void	execve_error(char **s_cmd); // to delete later

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

// Lexer
void lexer(char *input, t_token_list **tokenList, int *numTokens, int *error);
enum token_type determine_token_type(char *token_value);

// Execute
void execute_parse_tree(t_parse_tree *root, t_env *env);
char	*get_path(char *cmd, t_env *env);
char	*get_exec(char **path, int i, char *cmd);
void execute_pipeline(t_parse_tree *node, t_env *env);
void handle_redirection(t_parse_tree **node, int *fd_in, int *fd_out);
char *handle_here_doc(t_parse_tree **node, int *fd_in, int *fd_out);
void execute_command(char **args, int fd_in, int fd_out, t_env *env);
void execute_node(t_parse_tree *node, t_env *env);
void handle_global_env(t_parse_tree *node, char **args, int i, t_env *env);
void handle_quotes_global(t_parse_tree *node, char **args, int i, t_env *env);

// To delete later when working
void	execve_error(char **s_cmd);
void	free_array(char **array);
void	error_message(char *str);

// Libft
static void	ft_free(char **array, int j);
static size_t	n_words(const char *str, char c);
static size_t	size_word(const char *s, char c, int i);
char	**ft_split(char const *s, char c);
size_t	ft_strlen(const char *s);
int	ft_strcmp(const char *s1, const char *s2);
char	*ft_strjoin(char const *s1, char const *s2);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
static size_t	ft_special_cases(char const *s, unsigned int start, size_t len);
char	*ft_substr(char const *s, unsigned int start, size_t len);



t_env *create_env_var(const char *name, const char *value);
t_env *init_environment(char **envp);
t_env *find_env_var(t_env *head, const char *name);
void update_add_env_var(t_env **head, const char *name, const char *value);
void free_env(t_env *head);
char *get_env_var(t_env *head, const char *name);
char **env_list_to_array(t_env *head);
void    exec_cd(char **args, t_env *env);
void update_pwd(t_env **env, char *cwd);
int exec_builtins(char **args, t_env *env);
void    exec_echo(char **args);
void    exec_pwd(char **args);
void    exec_env(char **args, t_env *env);
void    exec_unset (char **args, t_env **env);

