#include "lexer.h"

void execute_word_node(t_parse_tree *node) {
    if (node == NULL || node->data == NULL)
    {
        fprintf(stderr, "Error: NULL node or data encountered in execute_word_node\n");
        exit(EXIT_FAILURE);
    }
    printf("I got this command - word %s\n", node -> data -> lexeme);
    // Execute a word node as a command
    execlp(node-> data -> lexeme, node-> data -> lexeme, NULL);
    // If execlp fails, print an error message
    perror("exec failed");
    exit(EXIT_FAILURE);
}

void execute_append_node(t_parse_tree *node) {
    // Open or create the file for appending
    int fd = open(node-> data -> lexeme, O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (fd == -1)
    {
        perror("open failed");
        exit(EXIT_FAILURE);
    }
    // Redirect stdout to the file
    dup2(fd, STDOUT_FILENO);
    close(fd);
    // Execute the left subtree (command)
    if (node->child != NULL)
        execute_parse_tree(node->child);
    if (node->sibling != NULL)
        execute_parse_tree(node->sibling);
}

void execute_pipe_node(t_parse_tree *node) {
    int pipefd[2];
    pid_t pid;

    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    { // Child process
        close(pipefd[0]); // Close unused read end
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to the write end of the pipe
        close(pipefd[1]); // Close the write end of the pipe
        if (node->child != NULL)
            execute_parse_tree(node->child); // Execute the left subtree (command)
    }
    else
    { // Parent process
        close(pipefd[1]); // Close unused write end
        dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to the read end of the pipe
        close(pipefd[0]); // Close the read end of the pipe
        if (node->sibling != NULL)
            execute_parse_tree(node->sibling); // Execute the right subtree (command)
    }
}

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
}