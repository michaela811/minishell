#include "lexer.h"

void execute_pipeline(t_parse_tree *node);
void handle_redirection(t_parse_tree **node, int *fd_in, int *fd_out);
void execute_command(char **args, int fd_in, int fd_out);
void execute_node(t_parse_tree *node);
void handle_global_env(t_parse_tree *node, char **args, int i);
void handle_quotes_global(t_parse_tree *node, char **args, int i);
/*
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
        else if (node->data->lexeme[0] == '$')
        {
          // heck if the argument starts with a $, indicating an environment variable
            // Get the name of the environment variable by removing the $ from the start
            char *env_var_name = node->data->lexeme + 1;

            // Get the value of the environment variable
            char *env_var_value = getenv(env_var_name);

            // If the environment variable is set, use its value as the argument
            if (env_var_value != NULL) {
                args[i] = env_var_value;
            }
            // If the environment variable is not set, use the original string as theargument
            else {
                args[i] = node->data->lexeme;
            }
            i++;
        }
        else if (node->data->lexeme[0] == '"')
        {
           // Remove the double quotes from the start and end of the string
           char *str = node->data->lexeme + 1;
           str[strlen(str) - 1] = '\0';

           // Create a buffer to hold the final string
           char buffer[1024] = "";

           // Loop over the string, looking for $variables
           char *start = str;
           while (1) {
               // Find the next $ in the string
               char *dollar = strchr(start, '$');

               // If there's no more $, append the rest of the string and break
               if (dollar == NULL) {
                   strcat(buffer, start);
                   break;
               }

               // Append the part of the string before the $
               strncat(buffer, start, dollar - start);

               // Get the name of the variable
               char *var_start = dollar + 1;
               char *var_end = strpbrk(var_start, " \t\n\"'$");
               if (var_end == NULL) var_end = var_start + strlen(var_start);
               char var_name[1024];
               strncpy(var_name, var_start, var_end - var_start);
               var_name[var_end - var_start] = '\0';

               // Get the value of the variable
               char *var_value = getenv(var_name);
               if (var_value != NULL) {
                   // Append the value of the variable
                   strcat(buffer, var_value);
               }

               // Continue from the end of the variable
               start = var_end;
           }

           // Use the final string as the argument
           args[i] = strdup(buffer);
           i++;
        }
            // If the argument does not start with a $, use it as is
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
*/

void handle_redirection(t_parse_tree **node, int *fd_in, int *fd_out)
{
    if ((*node)->data->type == RED_FROM)
    {
        *fd_in = open((*node)->child->data->lexeme, O_RDONLY);
        *node = (*node)->child;
    }
    else if ((*node)->data->type == RED_TO)
    {
        *fd_out = open((*node)->child->data->lexeme, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        *node = (*node)->child;
    }
    else if ((*node)->data->type == APPEND)
    {
        *fd_out = open((*node)->child->data->lexeme, O_WRONLY | O_CREAT | O_APPEND, 0644);
        *node = (*node)->child;
    }
}

void execute_command(char **args, int fd_in, int fd_out) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child process
        if (fd_in != 0) {
            dup2(fd_in, 0);
            close(fd_in);
        }
        if (fd_out != 1) {
            dup2(fd_out, 1);
            close(fd_out);
        }
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else { // Parent process
        waitpid(pid, &status, 0);
    }
}

void execute_node(t_parse_tree *node)
{
    int fd_in = 0;
    int fd_out = 1;
    char *args[10];
    int i = 0;

    if (node == NULL)
        return;
    while (node != NULL)
    {
        if (node->data != NULL)
        {
            if (node->data->type == RED_FROM || node->data->type == RED_TO
            || node->data->type == APPEND)
                handle_redirection(&node, &fd_in, &fd_out);
            else if (node->data->lexeme[0] == '$')
            {
                handle_global_env(node, args, i);
                i++;
            }
            else if (node->data->lexeme[0] == '"')
            {
                handle_quotes_global(node, args, i);
                i++;
            }
            else
                args[i++] = node->data->lexeme;
            //i++;
        }
        node = node->child;
    }
    args[i] = NULL;
    execute_command(args, fd_in, fd_out);
}

void handle_global_env(t_parse_tree *node, char **args, int i)
{
    char *env_var_name = node->data->lexeme + 1;
    char *env_var_value = getenv(env_var_name);
    if (env_var_value != NULL)
        args[i] = env_var_value;
    else
        args[i] = node->data->lexeme;
}


void handle_quotes_global(t_parse_tree *node, char **args, int i)
{
    char *str = node->data->lexeme + 1;
    str[strlen(str) - 1] = '\0';
    char buffer[1024] = "";
    char *start = str;
    while (1)
    {
        char *dollar = strchr(start, '$');
        if (dollar == NULL)
        {
            strcat(buffer, start);
            break;
        }
        strncat(buffer, start, dollar - start);
        char *var_start = dollar + 1;
        char *var_end = strpbrk(var_start, " \t\n\"'$");
        if (var_end == NULL)
            var_end = var_start + strlen(var_start);
        char var_name[1024];
        strncpy(var_name, var_start, var_end - var_start);
        var_name[var_end - var_start] = '\0';
        char *var_value = getenv(var_name);
        if (var_value != NULL)
            strcat(buffer, var_value);
        start = var_end;
    }
    args[i] = strdup(buffer);
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
            close(pipefd[1]);
        }

        //int in_fd = STDIN_FILENO;
        //int out_fd = pipefd[1];
        execute_node(node); // Execute the left command of the pipe
        //close(pipefd[1]); // Close write end after dup
        exit(EXIT_SUCCESS);
    } else if (pid > 0) {
        if (node->sibling != NULL)
        {
            close(pipefd[1]); // Close unused write end
            pid_t pid2 = fork();
            if (pid2 == 0) { // Child process for next command
                dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe read
                close(pipefd[0]); // Close read end after dup
                execute_pipeline(node->sibling->sibling); // Recursively handle the next part of the pipeline
                exit(EXIT_SUCCESS);
            } else if (pid2 > 0) { // Parent process
                close(pipefd[0]); // Close unused read end
                wait(NULL); // Wait for child process to finish
            } else {
                perror("fork");
                exit(EXIT_FAILURE);
            }
        }
        wait(NULL); // Wait for first child process to finish
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }
}