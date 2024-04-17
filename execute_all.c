#include "lexer.h"

void execute_pipeline(t_parse_tree *node);
void handle_redirection(t_parse_tree **node, int *fd_in, int *fd_out);
char *handle_here_doc(t_parse_tree **node, int *fd_in, int *fd_out);
void execute_command(char **args, int fd_in, int fd_out);
void execute_node(t_parse_tree *node);
void handle_global_env(t_parse_tree *node, char **args, int i);
void handle_quotes_global(t_parse_tree *node, char **args, int i);

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
        else if ((*node)->data->type == HERE_DOC) {
        char *filename = handle_here_doc(node, fd_in, fd_out);
        *fd_in = open(filename, O_RDONLY);
        *node = (*node)->child;
    }
}

char *handle_here_doc(t_parse_tree **node, int *fd_in, int *fd_out)
{
	char *filename = "/tmp/heredoc.txt";
    FILE *file = fopen(filename, "w"); // Error check!Unvalidated input in path value creation risks unintended file/directory access?
    if (file == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
   	char buffer[1024];
    char delim[1024];
    sprintf(delim, "%s\n", (*node)->child->data->lexeme);
   	while (fgets(buffer, sizeof(buffer), stdin) != NULL)
	{
    // Check for the delimiter
        if (strcmp(buffer, delim) == 0)
            break;
        fprintf(file, "%s", buffer);
   	}
	fclose(file);
    return (filename);
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
            || node->data->type == APPEND || node->data->type == HERE_DOC)
                handle_redirection(&node, &fd_in, &fd_out);
            else if (node->data->lexeme[0] == '$')
                handle_global_env(node, args, i++);
            else if (node->data->lexeme[0] == '"')
                handle_quotes_global(node, args, i++);
            else
                args[i++] = node->data->lexeme;
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

char *ft_strpbrk(char *str, char *delim)//MOVE IT TO FT_LIBFT
{
    while (*str)
    {
        if (strchr(delim, *str))
            break;
        str++;
    }
    if (*str)
        return (str);
    return (NULL);
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
