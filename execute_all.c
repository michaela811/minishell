#include "lexer.h"

void    exec_echo(char **args);

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
    char *buffer;
	char *filename = "/tmp/heredoc.txt";
    FILE *file = fopen(filename, "w"); // Error check!Unvalidated input in path value creation risks unintended file/directory access?
    if (file == NULL)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
   	while ((buffer = readline("heredoc> ")) != NULL)
	{
        if (strcmp(buffer, (*node)->child->data->lexeme) == 0)
            break;
        fprintf(file, "%s\n", buffer); //probably need to create this function?
        free(buffer);
   	}
	fclose(file);
    return (filename);
}

int exec_builtins(char **args, t_env *env)
{
    if (strcmp(args[0], "cd") == 0)
        return(exec_cd(args, env), 0);
    else if (strcmp(args[0], "pwd") == 0)
        return(exec_pwd(args), 0);
    else if (strcmp(args[0], "echo") == 0)
        return(exec_echo(args), 0);
    //else if (strcmp(args[0], "export") == 0)
       // return(exec_export(args), 0);
    else if (strcmp(args[0], "unset") == 0)
        return(exec_unset(args, env), 0);
    else if (strcmp(args[0], "env") == 0)
        return(exec_env(args, env), 0);
    //else if (strcmp(args[0], "exit") == 0) //already implemented
        //return(exec_exit(args), 0);
    return (1);
}

void execute_command(char **args, int fd_in, int fd_out, t_env *env) {
    pid_t pid;
    int status;
    char *path;
    char **environtment = env_list_to_array(env);

    if (exec_builtins(args, env) == 0)
        return ;
    else
    {
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        { // Child process
            if (fd_in != 0) {
                dup2(fd_in, 0);
                close(fd_in);
            }
            if (fd_out != 1) {
                dup2(fd_out, 1);
                close(fd_out);
            }
            path = get_path(args[0], env);
            if (execve(path, args, environtment) < 0)
            {
                perror("execve");
                exit(EXIT_FAILURE);
            }
        }
        else // Parent process
        waitpid(pid, &status, 0);
    }
}

void execute_node(t_parse_tree *node, t_env *env)
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
                handle_global_env(node, args, i++, env);
            else if (node->data->lexeme[0] == '"')
                handle_quotes_global(node, args, i++, env);
            else
                args[i++] = node->data->lexeme;
        }
        node = node->child;
    }
    args[i] = NULL;
    execute_command(args, fd_in, fd_out, env);
}

void handle_global_env(t_parse_tree *node, char **args, int i, t_env *env)
{
    char *env_var_name = node->data->lexeme + 1;
    char *env_var_value = get_env_var(env, env_var_name);
    if (env_var_value != NULL)
        args[i] = env_var_value;
    else
        args[i] = "";
}


void handle_quotes_global(t_parse_tree *node, char **args, int i, t_env *env)
{
    char *str = node->data->lexeme + 1;
    str[ft_strlen(str) - 1] = '\0';
    char buffer[1024] = "";
    char *start = str;
    while (1)
    {
        char *dollar = ft_strchr(start, '$');
        if (dollar == NULL)
        {
            strcat(buffer, start);
            break;
        }
        strncat(buffer, start, dollar - start);
        char *var_start = dollar + 1;
        char *var_end = ft_strpbrk(var_start, " \t\n\"'$");
        if (var_end == NULL)
            var_end = var_start + ft_strlen(var_start);
        char var_name[1024];
        strncpy(var_name, var_start, var_end - var_start);
        var_name[var_end - var_start] = '\0';
        char *var_value = get_env_var(env, var_name);
        if (var_value != NULL)
            strcat(buffer, var_value);
        start = var_end;
    }
    args[i] = ft_strdup(buffer);
}

void execute_parse_tree(t_parse_tree *root, t_env *env)
{
    if (root == NULL) {
        return;
    }
    if (root->sibling)
        execute_pipeline(root, env);//ADD a check that the child is already executed
    else
        execute_node(root->child, env);
}

void execute_pipeline(t_parse_tree *node, t_env *env)
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
        execute_node(node, env); // Execute the left command of the pipe
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
                execute_pipeline(node->sibling->sibling, env); // Recursively handle the next part of the pipeline
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

void update_pwd(t_env **env, char *cwd)
{
    update_add_env_var(env, "OLDPWD", cwd);
    cwd = getcwd(NULL, 0);  // Get the current working directory again
    update_add_env_var(env, "PWD", cwd);
    free(cwd);  // Free the current working directory string
}

void    exec_cd(char **args, t_env *env)
{
    char    *cwd;

    cwd = getcwd(NULL, 0);
    if (args[1] != NULL && args[2])
    {
        fprintf(stderr, "cd: too many arguments\n");
        return ;
    }
    else if (args[1] == NULL || strcmp(args[1], "~") == 0)
    {
        if (chdir(get_env_var(env, "HOME")) != 0)
            perror("chdir");
        return (update_pwd(&env, cwd), free(cwd));
    }
    else if (strcmp(args[1], "..") == 0)
    {
        if (chdir("..") != 0)
            perror("chdir");
        return (update_pwd(&env, cwd),free(cwd));
    }
    else if (chdir(args[1]) != 0)
        perror("chdir");
    return (update_pwd(&env, cwd),free(cwd));
}



void    exec_echo(char **args)//CHANGE IT TO FT_PRINTF AND FT_LIBFT
{
    int i = 1;
    if (strcmp(args[1], "-n") == 0)
        i++;
    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }
    if (strcmp(args[1], "-n") != 0)
        printf("\n");
}

void    exec_pwd(char **args)
{
    char    *cwd;

    cwd = getcwd(NULL, 0);
    if (cwd == NULL)
        perror("getcwd");
    else
    {
        printf("%s\n", cwd);
        free(cwd);
    }
}

void    exec_env(char **args, t_env *env)
{
    int i = 0;

    if (args[1] != NULL)
    {
        fprintf(stderr, "env: too many arguments\n");
        return ;
    }
    char **environtment = env_list_to_array(env);
    while (environtment[i] != NULL)
        printf("%s\n", environtment[i++]);
    return;
}

void    exec_unset (char **args, t_env **env)
{
    if (args[2] != NULL)
    {
        fprintf(stderr, "unset: too many arguments\n");
        return ;
    }
    t_env *current = *env;
    t_env *prev = NULL;

    while (current != NULL)
    {
        if (strcmp(current->name, args[1]) == 0)
        {
            if (prev == NULL)
                *env = current->next;
            else
                prev->next = current->next;
            free(current->name);
            free(current->value);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void exec_export_no_args(t_env *env)
{
    t_env *current = env;

    while (current != NULL)
    {
        printf("declare -x %s=\"%s\"\n", current->name, current->value);
        current = current->next;
    }
}

int var_control(char *args)
{
    int i = 0;
    if (args[i++] == '=')
    {
        fprintf(stderr, "'%s': not a valid identifier\n", args);
        return (-1);//think how to handle the error
    }
    while (args[i])
    {
        if(args[i++] == '=')
            return (0);
    }
    return(1);
}

void split_var(char *var, char **name, char **value)
{
    char *equals = strchr(var, '=');
    if (equals == NULL)
    {
        *name = strdup(var);

        *value = NULL;
    }
    else
    {
        *name = strndup(var, equals - var);
        *value = strdup(equals + 1);
    }
}

void    exec_export(char **args, t_env *env)
{
    char *name = NULL;
    char *value = NULL;

    if (args[1] != NULL)
        return(exec_export_no_args(env));
    if (var_control(args[1]) == 0)
    {
        split_var(args[1], name, value);
        update_add_env_var (&env, name ,value);
    }
    return;
}
