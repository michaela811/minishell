#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"
#include <errno.h>
#include <signal.h>
#include <limits.h>
#include <termios.h>
#include <sys/ioctl.h>

/*#define MY_MALLOC(size) custom_malloc(size, __FILE__, __LINE__)
#define MY_FREE(ptr) custom_free(ptr, __FILE__, __LINE__)*/

enum token_type
{
    PIPE = 1,   // 1
    WORD,       // 2
    RED_TO,     // 3 >
    RED_FROM,   // 4 <
    APPEND,     // 5 >>
    HERE_DOC,   // 6 <<
};

/*typedef struct MemoryBlock {
    void *address;
    size_t size;
    const char *file;
    int line;
    struct MemoryBlock *next;
} MemoryBlock;*/

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

typedef struct s_exec_vars
{
    int fd_in;
    int fd_out;
    char **args;//char *args[10];
    int i;
    int error;
} t_exec_vars;

// Building struct to free when exiting
typedef struct s_free_data {
    t_parse_tree *tree;
    t_parse_tree *tree_start;
    t_env *env;
    t_token_list *token_list;
    t_token_list *token_list_start;
    char **environment;
} t_free_data;

// Return values
#define SYNTAX_ERROR -1
#define SUBTREE_OK 0
#define MEMORY_ERROR 1
//#define PARSING_ERROR 2

// Global variable for signal handling
extern int		g_last_exit_status;

//MAIN
void			handle_signal(int signal);
void	        handle_input(char *input, t_free_data *exec_data);
void	        handle_preprocess_input(char *input, t_free_data *exec_data);
void	        handle_parse_tree(t_free_data *exec_data);

// Allocs and frees
t_parse_tree*	alloc_parse_tree();
void			free_parse_tree(t_parse_tree *tree);
void			free_token(t_token* tok);
void			free_token_list(t_token_list* list);
void	        free_command_data(t_free_data *exec_data);
void	        free_exit_data(t_free_data *exec_data);

// Errors
void			handle_memory_error(t_token **token_list, int num_tokens);
void			exit_function(int i); // to delete later
void			execve_error(char **s_cmd); // to delete later
void            printf_global_error(int status, unsigned int fd, char *format, ...);

// Parsing
void			link_node(t_parse_tree **current, t_parse_tree *newNode);
int				is_cmd_word(t_token_list **tok, t_parse_tree **cmd_word_node);
int				is_simple_command(t_token_list **tok, t_parse_tree **new);
int				is_io_redirect(t_token_list **tok, t_parse_tree **redirect_node);
bool			is_redirection_token(enum token_type type);
int				is_cmd_prefix(t_token_list **tok, t_parse_tree **prefix_node);
int				is_cmd_suffix(t_token_list **tok, t_parse_tree **suffix_node);
void			link_pipe(t_parse_tree **current, t_parse_tree *newNode);
int             is_pipe_sequence(t_free_data *exec_data);

// Printing
void			print_parse_tree(t_parse_tree* tree, int depth);

// Testing
void			test_parser(); // to delete later

// Tokens
t_token_list	*create_token(enum token_type type, char* lexeme); //probably to delete later
void			add_token(t_token_list** list, enum token_type type, char* lexeme); // same as above

// Lexer helper
int				count_additional_chars(char *input, const char *delim);
char			*preprocess_input(char *str, const char *delim);
void			process_input_str(char *str, const char *delim, char *dest);

// Lexer
int				lexer(char *input, t_token_list **tokenList);
int				create_and_add_token(char **tokenValue, t_token_list **tokenList,
t_token_list	**current);//, int *error);
t_token			*create_token_lexer(char *tokenValue);
t_token_list	*create_node_lexer(t_token *newToken);

enum token_type	determine_token_type(char *token_value);

// Execute
int             execute_parse_tree(t_free_data *exec_data);
int             exec_exit(t_exec_vars *vars, t_free_data *exec_data);
int             get_path(char *cmd, t_env *env, char **exec);
int             get_exec(char **path, int i, char *cmd, char **exec);
int             handle_child_process(int *pipefd, t_free_data *exec_data);
int             handle_sibling_process(int *pipefd, t_free_data *exec_data);
int             handle_parent_process(int *pipefd, pid_t pid, t_free_data *exec_data);
int             execute_pipeline(t_free_data *exec_data);

void            handle_redirection(t_parse_tree **node, t_exec_vars *vars);
void            handle_redirection_from(t_parse_tree **node, t_exec_vars *vars);
void            handle_redirection_to(t_parse_tree **node, t_exec_vars *vars);
void            handle_redirection_append(t_parse_tree **node, t_exec_vars *vars);
void            handle_redirection_here_doc(t_parse_tree **node, t_exec_vars *vars);
char            *handle_here_doc(t_parse_tree **node, t_exec_vars *vars);

int             exec_builtins(t_exec_vars *vars, t_free_data *exec_data);
int             handle_child_cmd(t_exec_vars *vars, t_env **env, char **environment);
int             handle_fork(t_exec_vars *vars, t_env **env, char **environment);
int             execute_command(t_exec_vars *vars, t_free_data *exec_data);
void            init_exec_vars(t_exec_vars *vars);
void            handle_node_data(t_parse_tree **node, t_exec_vars *vars, t_env **env);
int             split_variable(char *arg, int i, t_exec_vars *vars);
int             execute_node(t_free_data *exec_data);
void            handle_global_env(t_parse_tree **node, char **args, int i, t_env **env);
//void handle_quotes_global(t_parse_tree *node, char **args, int i, t_env **env);
void            handle_quotes_global(t_parse_tree **node, char **args, int i, t_env **env);
void	handle_quotes_glob(char **arg, t_env **env, int *error);
void            handle_dollar_sign(char **start, char *buffer, t_env **env);//, int *k);

// To delete later when working
void            execve_error(char **s_cmd);
void            free_array(char **array);
int             error_message(char *str);

// Libft
/*
static void     ft_free(char **array, int j);
static size_t   n_words(const char *str, char c);
static size_t   size_word(const char *s, char c, int i);
char            **ft_split(char const *s, char c);
size_t          ft_strlen(const char *s);
int             ft_strcmp(const char *s1, const char *s2);
char            *ft_strjoin(char const *s1, char const *s2);
void            ft_putstr_fd(char *s, int fd);
void            ft_putendl_fd(char *s, int fd);
static size_t   ft_special_cases(char const *s, unsigned int start, size_t len);
char            *ft_substr(char const *s, unsigned int start, size_t len);
*/



t_env           *create_env_var(const char *name, const char *value);
t_env           *init_environment(char **envp);
t_env           *find_env_var(t_env *head, const char *name);
int             update_add_env_var(t_env **head, const char *name,
                const char *value);
void            free_env(t_env *head);
int             count_env_list(t_env *head);
char            *create_env_str(t_env *current);
void            free_env_array(char **env_array);
char            *get_env_var(t_env *head, const char *name);
char            **env_list_to_array(t_env *head);
int             exec_cd(char **args, t_env **env);
int             update_pwd(t_env **env, char *cwd);
int             change_directory_and_update(char *path, t_env **env, char *cwd);
int             exec_echo(t_exec_vars *vars);
char            *handle_quotes_echo(const char *input, int *error);
void            process_args(char **args, int *error);
int             exec_pwd(void);
int             exec_dollar_pwd(void);
int             exec_env(char **args, char **environment);
int             exec_unset (char **args, t_env **env);
void            exec_export_no_args(t_env *env);
int             var_control(char *args);
int             split_var(char *var, char **name, char **value);
int             exec_export(char **args, t_env **env);
int             export_quotes(char *input, char **output);
int             exec_global_env(t_exec_vars *vars, t_env **env);

t_free_data     *init_command_data(char **envp);

/*
void check_for_memory_leaks();
void custom_free(void *ptr, const char *file, int line);
void *custom_malloc(size_t size, const char *file, int line);*/
