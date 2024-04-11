#include "lexer.h"

void execute_pipeline(t_parse_tree *node);

void execute_node(t_parse_tree *node)
{
    char *args[10]; // Adjust size as needed, may check how many is needed just in case?
    int i = 0;
    pid_t pid;
    int status;
    int fd_in = 0;
    int fd_out = 1;

    if (node == NULL) {
        return;
    }
    while (node != NULL)
    {
    if (node->data != NULL)
    {
        if (node->data->type == RED_FROM)
        {
            fd_in = open(node->child->data->lexeme, O_RDONLY);
            node = node->child;
        }
        else if (node->data->type == RED_TO)
        {
            fd_out = open(node->child->data->lexeme, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            node = node->child;
        }
        else if (node->data->type == APPEND)
        {
            fd_out = open(node->child->data->lexeme, O_WRONLY | O_CREAT | O_APPEND, 0644);
            node = node->child;
        }
        else
        {
            args[i] = node->data->lexeme;
            i++;
        }
    }
    node = node->child;
    }
    args[i] = NULL;
    // Fork a new process
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child process
        // Execute the command
        if (fd_in != 0)
        {
            dup2(fd_in, 0);
            close(fd_in);
        }
        if (fd_out != 1)
        {
            dup2(fd_out, 1);
            close(fd_out);
        }
		for (i = 0; args[i] != NULL; i++) {
    	printf("args[%d] = %s\n", i, args[i]);
		fflush(stdout);
		}
        execvp(args[0], args);

        // If execvp fails, print error and exit
        perror("execvp");
        exit(EXIT_FAILURE);
    } else { // Parent process
        // Wait for child to finish
        waitpid(pid, &status, 0);
    }
}

void execute_parse_tree(t_parse_tree *root)
{
    if (root == NULL) {
        return;
    }
	//while (root != NULL)
    //{
        if (root->sibling)
            execute_pipeline(root);//ADD a check that the child is already executed
        else
            execute_node(root->child);
    //}
    // Execute sibling nodes - pipes recursively, if it exists
    //if (root->sibling != NULL)
        //execute_parse_tree(root->sibling->sibling);
}

void execute_pipeline(t_parse_tree *node)
{
    int pipefd[2];
    pid_t pid;

	if (node == NULL) {
        return;
    }

    if (node->sibling != NULL) {
        if (pipe(pipefd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    pid = fork();
    if (pid == 0)
    { // Child process
	if (node->sibling != NULL) {
            close(pipefd[0]); // Close unused read end
            dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe write
        }

        //int in_fd = STDIN_FILENO;
        //int out_fd = pipefd[1];
        execute_node(node); // Execute the left command of the pipe
        close(pipefd[1]); // Close write end after dup
        exit(EXIT_SUCCESS);
    } else if (pid > 0) {
        close(pipefd[1]); // Close unused write end

        if (node->sibling) {
            dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe read
            close(pipefd[0]); // Close read end after dup
            execute_pipeline(node->sibling->sibling); // Recursively handle the next part of the pipeline
        }

        wait(NULL); // Wait for child process to finish
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }
}