#include "lexer.h"

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
        else if (node->data->type == APPEND) //Check this, if there is more to it?
        {
            fd_out = open(node->child->data->lexeme, O_WRONLY | O_CREAT | O_APPEND, 0644);
            node = node->child;
        }
		else if (node->data->type == HERE_DOC)
		{
			char *filename = "/tmp/heredoc.txt"; // Temporary file to hold heredoc content
            FILE *file = fopen(filename, "w"); // Error check!Unvalidated input in path value creation risks unintended file/directory access?
            if (file == NULL) {
                perror("fopen");
                exit(EXIT_FAILURE);
            }
    		// Write the content of the heredoc to the file
   			char buffer[1024];
   			while (fgets(buffer, sizeof(buffer), stdin) != NULL)
			{
        	// Check for the delimiter
				buffer[strcspn(buffer, "\n")] = 0;
        		if (strcmp(buffer, node->child->data->lexeme) == 0) 
            		break;
        		fprintf(file, "%s", buffer);
   			}
			fclose(file);
			fd_in = open(filename, O_RDONLY);
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

    // Execute current node
    execute_node(root);

    // Execute sibling nodes - pipes recursively, if it exists
    if (root->sibling != NULL)
        execute_parse_tree(root->sibling->sibling);
}