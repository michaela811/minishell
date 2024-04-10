#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
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


void execute_command_with_redirection(t_parse_tree *node, int in_fd, int out_fd);
void execute_pipeline(t_parse_tree *node);
void handle_redirection(t_parse_tree *node, int *in_fd, int *out_fd);
void execute_tree(t_parse_tree *node);

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


t_parse_tree* alloc_parse_tree()
{
    t_parse_tree* tree;
    tree = malloc(sizeof(t_parse_tree));
    if (tree == NULL)
        return NULL;
    tree->data = NULL;
    tree->child = NULL;
    tree->sibling = NULL;
    return tree;
}
void free_token(t_token* tok)
{
    if (tok == NULL)
        return;
    if (tok->lexeme == NULL)
        return;
    free(tok->lexeme);
    tok->lexeme = NULL;
    free(tok);
    tok = NULL;
}

void free_parse_tree(t_parse_tree *tree)
{
    if (tree == NULL) return;
    t_parse_tree* sibling;
    t_parse_tree* child;
    child = tree->child;
    sibling = tree->sibling;
    if (sibling != NULL)
        free_parse_tree(sibling);
    if (child != NULL)
        free_parse_tree(child);
    if (tree->data != NULL)
    {
        free_token(tree->data);
        tree->data = NULL;
    }
    if(tree != NULL)
    {
        free(tree);
        tree = NULL;
    }
}


void link_node(t_parse_tree **current, t_parse_tree *newNode)
{
    if (current == NULL || newNode == NULL) return; // Basic validation
    if (*current == NULL) {
        // No current node, directly link the new node
        *current = newNode;
    } else {
        // Find the last node in the current sequence of siblings
        t_parse_tree *last = *current;
        while (last->child != NULL) {
            last = last->child;
        }
        // Link the new node as a sibling of the last node
        last->child = newNode;
    }
}

int is_cmd_word(t_token_list **tok, t_parse_tree **cmd_word_node)
{
    if (*tok != NULL && (*tok)->token->type == WORD)
	{
		*cmd_word_node = alloc_parse_tree();
    	if (*cmd_word_node == NULL)
        	return(MEMORY_ERROR);
    	(*cmd_word_node)->data = (*tok)->token; // Set the WORD token to this node
    	*tok = (*tok)->next; // Move to the next token
	}
    return(SUBTREE_OK);
}

int is_simple_command(t_token_list **tok, t_parse_tree **new)
{
    *new = alloc_parse_tree();
    if (*new == NULL)
        return(MEMORY_ERROR);
    if(is_cmd_prefix(tok, &((*new)->child)) != SUBTREE_OK)// && (*new)->child != NULL)
        return(SYNTAX_ERROR);//COULD BE ALSO MEMORY_ERROR
    t_parse_tree *cmd_word_node = NULL;
    if(is_cmd_word(tok, &cmd_word_node) != SUBTREE_OK)
        return(free_parse_tree(*new), SYNTAX_ERROR);//COULD BE ALSO MEMORY_ERROR
	if (cmd_word_node == NULL)// PROBABLY CHECK WHETHER THE TOKEN LIST IS EMPTY
		return(SUBTREE_OK);
    link_node(&((*new)->child), cmd_word_node);//change to (*new)->child
    t_parse_tree *cmd_suffix_node = NULL;// Optionally parse and attach cmd_suffix
    if(is_cmd_suffix(tok, &cmd_suffix_node) == SUBTREE_OK)
    {
        if(cmd_suffix_node && cmd_suffix_node->child != NULL)
        link_node(&((*new)->child), cmd_suffix_node);//cmd_word_node->child = cmd_suffix_node;  // Attach suffix as sibling to cmd_word
    }
    else
        return(free_parse_tree(*new), SYNTAX_ERROR);
    return(SUBTREE_OK);
}

int is_io_redirect(t_token_list **tok, t_parse_tree **redirect_node)
{
    if (*tok == NULL || (*tok)->token == NULL)
        return(SYNTAX_ERROR);
    if (is_redirection_token((*tok)->token->type)) // Check if the current token is a redirection operator
    {
        *redirect_node = alloc_parse_tree();
        if (*redirect_node == NULL)
            return(MEMORY_ERROR);
        (*redirect_node)->data = (*tok)->token;
        *tok = (*tok)->next;
        if (*tok == NULL || (*tok)->token->type != WORD)
            return(free_parse_tree(*redirect_node), SYNTAX_ERROR);
        (*redirect_node)->child = alloc_parse_tree();// Create a child node for the filename
        if ((*redirect_node)->child == NULL)
            return(free_parse_tree(*redirect_node), MEMORY_ERROR);
        (*redirect_node)->child->data = (*tok)->token; // Set the filename as the data for the child node
        *tok = (*tok)->next;// Move past the filename token
        return(SUBTREE_OK);
    }
    return(SYNTAX_ERROR);
}

bool is_redirection_token(enum token_type type)
{
    if (type == RED_FROM || type == RED_TO || type == APPEND || type == HERE_DOC)
        return true;
    return false;
}

int is_cmd_prefix(t_token_list **tok, t_parse_tree **prefix_node)
{
    if (*tok != NULL && ((*tok)->token->type == ASSIGNMENT_WORD ||
           is_redirection_token((*tok)->token->type)))
    {
        *prefix_node = alloc_parse_tree();
        if (prefix_node == NULL)
            return(MEMORY_ERROR);
    }
    while (*tok != NULL && ((*tok)->token->type == ASSIGNMENT_WORD ||
           is_redirection_token((*tok)->token->type)))
    {
        if (is_redirection_token((*tok)->token->type))
        {
            t_parse_tree *redirect_node = NULL;
            if(is_io_redirect(tok, &redirect_node) != SUBTREE_OK)
                return(free_parse_tree(*prefix_node), SYNTAX_ERROR);//COULD BE ALSO MEMORY_ERROR
            link_node(prefix_node, redirect_node); // Link the redirect_node appropriately (e.g., as the child or sibling)
        }
        else if ((*tok)->token->type == ASSIGNMENT_WORD)
        {
            t_parse_tree *assignment_node = alloc_parse_tree();// Handle ASSIGNMENT_WORD directly here
            if (assignment_node == NULL)
                return(free_parse_tree(*prefix_node), MEMORY_ERROR);
            assignment_node->data = (*tok)->token; // Set the token to this node
            *tok = (*tok)->next; // Move to the next token
            link_node(prefix_node, assignment_node); // Link the assignment_node
        }
    }
    return(SUBTREE_OK);
}

int is_cmd_suffix(t_token_list **tok, t_parse_tree **suffix_node)
{
    if (*tok != NULL && ((*tok)->token->type == WORD ||
           is_redirection_token((*tok)->token->type)))
    {
        *suffix_node = alloc_parse_tree();
        if (*suffix_node == NULL)
            return(MEMORY_ERROR);
    }
    while (*tok != NULL && ((*tok)->token->type == WORD ||
           is_redirection_token((*tok)->token->type)))
    {
        if (is_redirection_token((*tok)->token->type))
        {
            t_parse_tree *redirect_node = NULL;
            if(is_io_redirect(tok, &redirect_node) != SUBTREE_OK)
                return(free_parse_tree(*suffix_node), SYNTAX_ERROR);//COULD BE ALSO MEMORY_ERROR
            link_node(&(*suffix_node)->child, redirect_node); // Link the redirect_node appropriately (e.g., as the child or sibling)
        }
        else if ((*tok)->token->type == WORD)
        {
            t_parse_tree *word_node = NULL;
            if(is_cmd_word(tok, &word_node)!= SUBTREE_OK)
                return(free_parse_tree(*suffix_node), SYNTAX_ERROR);//COULD BE ALSO MEMORY_ERROR
            link_node(&(*suffix_node)->child, word_node); // Link the redirect_node appropriately (e.g., as the child or sibling)
        }
    }
    return(SUBTREE_OK);
}

void link_pipe(t_parse_tree **current, t_parse_tree *newNode)
{
    if (current == NULL || newNode == NULL) return; // Basic validation
    if ((*current)->sibling == NULL) {
        // No current node, directly link the new node
        (*current)->sibling  = newNode;
    } else {
        // Find the last node in the current sequence of siblings
        t_parse_tree *last = *current;
        while (last->sibling != NULL) {
            last = last->sibling;
        }
        // Link the new node as a sibling of the last node
        last->sibling = newNode;
    }
}

int is_pipe_sequence(t_token_list **tok, t_parse_tree **new)//, int *status)
{
    if (*tok == NULL || (*tok)->token == NULL)
        return(SYNTAX_ERROR);
    *new = alloc_parse_tree(); // Allocate the root node for the pipe sequence
    if (*new == NULL)
        return(MEMORY_ERROR);
    t_parse_tree *current_command = NULL;
    if(is_simple_command(tok, &current_command) != SUBTREE_OK || !current_command->child)
        return(free_parse_tree(*new), SYNTAX_ERROR); //COULD BE ALSO MEMORY_ERROR
    (*new)->child = current_command; // Attach the first command as a child of the pipe sequence
    while (*tok != NULL && (*tok)->token->type == PIPE)// Iteratively look for PIPE tokens and subsequent simple_commands
    {
        t_parse_tree *pipe_node = alloc_parse_tree();
        if (pipe_node == NULL)
            return(free_parse_tree(current_command), free_parse_tree(*new), MEMORY_ERROR);
        pipe_node->data = (*tok)->token;
        *tok = (*tok)->next; // Move past the PIPE token
        link_pipe(new, pipe_node);
        t_parse_tree *next_command = NULL;
        if(is_simple_command(tok, &next_command) != SUBTREE_OK || !next_command->child)
            return(free_parse_tree(next_command), SYNTAX_ERROR);
        link_pipe(new, next_command);//pipe_node->child = next_command;// Link the next_command as a sibling of the current command
    }
    return(SUBTREE_OK);
}


t_token_list* create_token(enum token_type type, char* lexeme)
{
    t_token* new_token = malloc(sizeof(t_token));
    if (!new_token) {
        perror("Failed to allocate memory for token");
        exit(MEMORY_ERROR);
    }
    new_token->lexeme = strdup(lexeme);
    new_token->type = type;

    t_token_list* new_list_node = malloc(sizeof(t_token_list));
    if (!new_list_node) {
        free(new_token->lexeme);
        free(new_token);
        perror("Failed to allocate memory for token list node");
        exit(MEMORY_ERROR);
    }
    new_list_node->token = new_token;
    new_list_node->next = NULL;
    return new_list_node;
}

void add_token(t_token_list** list, enum token_type type, char* lexeme) {
    t_token_list* new_node = create_token(type, lexeme);
    if (!*list) {
        *list = new_node;
    } else {
        t_token_list* current = *list;
        while (current->next) {
            current = current->next;
        }
        current->next = new_node;
    }
}

void print_parse_tree(t_parse_tree* tree, int depth) {
    if (!tree) return;

    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    if (tree->data) {
        printf("%s (%d)\n", tree->data->lexeme, tree->data->type);
    } else {
        printf("NULL\n");
    }

    print_parse_tree(tree->child, depth + 1);
    print_parse_tree(tree->sibling, depth);
}

void free_token_list(t_token_list* list)
{
    t_token_list* current = list;
    t_token_list* next;

    while (current != NULL)
    {
        next = current->next;
        if (current->token->lexeme != NULL)
        {
            free(current->token->lexeme);
            current->token->lexeme = NULL; // Prevent double-free
        }
        if (current->token != NULL)
        {
            free(current->token);
            current->token = NULL; // Prevent double-free
        }
        if (current != NULL)
        {
            free(current);
            current = NULL; // Prevent double-free
        }
        current = next;
    }

    list = NULL; // Set the list pointer to NULL after freeing
}

#define MAXARGS 100// Leter to understand how many we need


int count_nodes(t_parse_tree *root, int *count)
{
    // Base case: if the node is NULL, return
    if (root == NULL)
        return (0);
    // If the node has data, increment the count
    if (root->data != NULL)
        (*count)++;
    // Recursively count nodes from the children and siblings
    count_nodes(root->child, count);
    count_nodes(root->sibling, count);
    return *count;
}

void collect_nodes(t_parse_tree *root, t_parse_tree **array, int *index)
{
    // Base case: if the node is NULL, return
    if (root == NULL)
        return;
    // If the node has data, add it to the array
    if (root->data != NULL)
    {
        array[*index] = root;
        (*index)++;
    }
    // Recursively collect nodes from the children and siblings
    collect_nodes(root->child, array, index);
    collect_nodes(root->sibling, array, index);
}

t_parse_tree **build_array(t_parse_tree *root)
{
    int count = 0;
    count_nodes(root, &count);

    t_parse_tree **array = malloc(count * sizeof(t_parse_tree *));
    if (array == NULL) {
        // Handle error
        free_parse_tree(root);
        return NULL;
    }

    int index = 0;
    collect_nodes(root, array, &index);

    int i = 0;
    //free_parse_tree(root);
    execute_tree(*array);
    //while (i < count)
    //    execute_node(array[i++]);
    return array;
}

void execute_command_with_redirection(t_parse_tree *node, int in_fd, int out_fd)
{
    // Similar to execute_command, but with added file descriptor redirections
    if (!node || node->data->type != WORD) return;

    // Collect command and its arguments
    char *argv[MAXARGS];
    int argc = 0;
    for (t_parse_tree *current = node; current != NULL && current->data->type == WORD; current = current->child) {
        argv[argc++] = current->data->lexeme;
    }
    argv[argc] = NULL; // Null-terminate the argv array

    // Execute the command
    pid_t pid = fork();
    if (pid == 0) { // Child process
        if (in_fd != -1) {
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }
        if (out_fd != -1) {
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }
        execvp(argv[0], argv);
        fprintf(stderr, "Failed to execute '%s'\n", argv[0]);
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Parent process waits for the child to complete
        wait(NULL);
    } else {
        // Fork failed
        perror("fork");
    }
}

void execute_pipeline(t_parse_tree *node) {
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == 0) { // Child process
        close(pipefd[0]); // Close unused read end
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe write
        close(pipefd[1]); // Close write end after dup

        int in_fd = STDIN_FILENO;
        int out_fd = pipefd[1];
        execute_command_with_redirection(node, in_fd, out_fd); // Execute the left command of the pipe
        exit(EXIT_SUCCESS);
    } else if (pid > 0) {
        close(pipefd[1]); // Close unused write end
        dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe read
        close(pipefd[0]); // Close read end after dup

        if (node->sibling) {
            execute_pipeline(node->sibling); // Recursively handle the next part of the pipeline
        }

        wait(NULL); // Wait for child process to finish
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }
}

void handle_redirection(t_parse_tree *node, int *in_fd, int *out_fd) {
    switch (node->data->type) {
        case RED_FROM: // Input redirection
            *in_fd = open(node->child->data->lexeme, O_RDONLY);
            break;
        case RED_TO: // Output redirection
            *out_fd = open(node->child->data->lexeme, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            break;
        case APPEND: // Output redirection with append
            *out_fd = open(node->child->data->lexeme, O_WRONLY | O_CREAT | O_APPEND, 0644);
            break;
        case HERE_DOC:
            // Handle here document...
            break;
        default:
            break;
    }
}

void execute_tree(t_parse_tree *node)
{
    if (!node) return;
    t_parse_tree *current = node;
    while (current != NULL)
    {
        if (current->data == NULL) {
            if (current->sibling) {
                current = current->sibling;
                continue;
            } else {
                current = current->child;
                continue;
            }
        }
        switch (current->data->type) {
        case WORD:
            if (current->sibling && current->sibling->data->type == PIPE) {
                execute_pipeline(current);
            } else {
                int in_fd = -1, out_fd = -1;
                if (current->sibling) {
                    handle_redirection(current->sibling, &in_fd, &out_fd);
                }
                execute_command_with_redirection(current, in_fd, out_fd);
                if (in_fd != -1) close(in_fd);
                if (out_fd != -1) close(out_fd);
            }
            break;
        // Add cases for other node types here
        default:
            break;
        }
        current = current->sibling; // Move to the next sibling
    }
    // Assuming siblings are sequential commands (not piped)
    if (node->sibling && node->sibling->data->type != PIPE) {
        execute_tree(node->sibling);
    }
}
/*
void execute_parse_tree(t_parse_tree *root)
{
    if (root == NULL) // check whether the root is NULL and it is necessary?
    {
        fprintf(stderr, "Error: NULL node encountered in execute_parse_tree\n");
        return;
    }
    t_parse_tree *current = root;
    while (current != NULL)
    {
        if (current -> data == NULL)
        {
            current = current -> child;
            continue;
        }
        switch (current -> data -> type)
        {
            case WORD:
                execute_word_node(current);
                break;
            case APPEND:
                execute_append_node(current);
                break;
            case PIPE:
                execute_pipe_node(current);
                break;
            default:
                fprintf(stderr, "Unknown token type encountered\n");
                exit(EXIT_FAILURE);
        }
        if (current -> child != NULL)
            execute_parse_tree(current -> child);
        current = current -> sibling;
    }
}*/

void main()
{
    t_token_list* list = NULL;
    int status = 0;
    // Example command: echo "Hello, World!" > output.txt
    add_token(&list, WORD, "cat");
    add_token(&list, RED_TO, ">");
    add_token(&list, WORD, "out1");
    add_token(&list, RED_FROM, "<");
    add_token(&list, WORD, "fileinput.txt");
    add_token(&list, RED_TO, ">");
    add_token(&list, WORD, "out2");
    add_token(&list, PIPE, "|");
    add_token(&list, WORD, "greb");
    add_token(&list, WORD, "123");
    add_token(&list, PIPE, "|");
    add_token(&list, WORD, "greb");
    add_token(&list, WORD, "123");

    t_parse_tree* root = NULL;
    if (is_pipe_sequence(&list, &root) == SUBTREE_OK)//, &status);
    {
        printf("Parse tree:\n");
        print_parse_tree(root, 0);
    }
    else
    {
        //free_parse_tree(root);//MAYBE NOT NEEDED IF WAS FREED IN IS_PIPE_SEQUENCE
        printf("Parser returned an error: %d\n", status);
    }
    //t_parse_tree **array = build_array(root);
    execute_tree(root);
    // Free resources
    free_token_list(list);
	free_parse_tree(root);
    // Note: You also need to free the token list, which is not covered here
}

/*
void execute_tree(t_parse_tree *root) {
    if (root == NULL) {
        return;
    }

    if (root->data->type == COMMAND) {
        char *args[] = collect_args(root);
        if (has_pipe_sibling(root)) {
            int pipefd[2];
            pipe(pipefd);
            redirect_stdout_to(pipefd[1]);
            execvp(args[0], args);
            execute_tree(root->sibling->sibling); // Execute command on right side of pipe
            restore_stdout();
        } else if (has_redirection_sibling(root)) {
            // Handle redirection...
        } else {
            execvp(args[0], args);
        }
    } else {
        execute_tree(root->child);
    }
}

void search_for_pipe(t_parse_tree *node)
{
    if (!node)
        return;

    if (node->data->type == PIPE)
    {
        execute_pipe(node);
    }
    if (node->sibling)
        search_for_pipe(node->sibling);
}

void execute_pipe(t_parse_tree *node)
{
    int fd[2];
    pid_t pid;
    int status;

    if (pipe(fd) == -1)
    {
        perror("pipe");
        return;
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        execute_node(node->child);
        exit(EXIT_SUCCESS);
    }
    else
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        waitpid(pid, &status, 0);
        execute_node(node->sibling);
    }
}

void execute_node(t_parse_tree *node)
{
    if (!node)
        return;

    // Execute a simple command
    if (node->data->type == WORD)
    {
        // Assuming the command might have arguments, collect them
        char *argv[MAXARGS]; // MAXARGS needs to be defined based on your requirements
        int argc = 0;
        argv[argc++] = node->data->lexeme; // The command itself

        // Collect arguments
        t_parse_tree *arg = node->child;
        while (arg && arg->data->type == WORD) {
            argv[argc++] = arg->data->lexeme;
            arg = arg->sibling; // Assuming siblings represent sequential WORD tokens
        }
        argv[argc] = NULL; // NULL-terminate the argument list

        // Now fork and execute the command with arguments
        pid_t pid = fork();
        if (pid == 0) { // Child process
            execvp(argv[0], argv);
            perror("execvp"); // If execvp returns, there was an error
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            int status;
            waitpid(pid, &status, 0); // Wait for the child to finish
        } else {
            perror("fork");
            return;
        }
    }

    // Recursively handle siblings for sequential execution
    if (node->sibling) {
        execute_node(node->sibling);
    }
}*/

