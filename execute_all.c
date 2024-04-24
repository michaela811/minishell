#include "lexer.h"

void handle_redirection_from(t_parse_tree **node, t_exec_vars *vars)
{
    vars->fd_in = open((*node)->child->data->lexeme, O_RDONLY);
    if (vars->fd_in == -1)
    {
        perror("open");
        vars->error = 1;
    }
    *node = (*node)->child;
    vars->i++;
}

void handle_redirection_to(t_parse_tree **node, t_exec_vars *vars)
{
    vars->fd_out = open((*node)->child->data->lexeme, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (vars->fd_out == -1)
    {
        perror("open");
        vars->error = 1;
    }
    *node = (*node)->child;
    vars->i++;
}

void handle_redirection_append(t_parse_tree **node, t_exec_vars *vars)
{
    vars->fd_out = open((*node)->child->data->lexeme, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (vars->fd_out == -1)
    {
        perror("open");
        vars->error = 1;
    }
    *node = (*node)->child;
    vars->i++;
}

void handle_redirection_here_doc(t_parse_tree **node, t_exec_vars *vars)
{
    char *filename = handle_here_doc(node, vars);
    if (vars->error)
        return ;
    vars->fd_in = open(filename, O_RDONLY);
    if (vars->fd_in == -1)
    {
        perror("open");
        vars->error = 1;
    }
    *node = (*node)->child;
    vars->i++;
}

void handle_redirection(t_parse_tree **node, t_exec_vars *vars)
{
    if ((*node)->data->type == RED_FROM)
        return handle_redirection_from(node, vars);
    else if ((*node)->data->type == RED_TO)
        return handle_redirection_to(node, vars);
    else if ((*node)->data->type == APPEND)
        return handle_redirection_append(node, vars);
    else if ((*node)->data->type == HERE_DOC)
        return handle_redirection_here_doc(node, vars);
}
/*
int handle_redirection(t_parse_tree **node, int *fd_in, int *fd_out)
{
    int error = 0;

    if ((*node)->data->type == RED_FROM)
    {
        *fd_in = open((*node)->child->data->lexeme, O_RDONLY);
        if (*fd_in == -1)
            return(perror("open"), 1);
        *node = (*node)->child;
    }
    else if ((*node)->data->type == RED_TO)
    {
        *fd_out = open((*node)->child->data->lexeme, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (*fd_out == -1)
            return(perror("open"), 1);
        *node = (*node)->child;
    }
    else if ((*node)->data->type == APPEND)
    {
        *fd_out = open((*node)->child->data->lexeme, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (*fd_out == -1)
            return(perror("open"), 1);
        *node = (*node)->child;
    }
    else if ((*node)->data->type == HERE_DOC)
    {
        char *filename = handle_here_doc(node, fd_in, fd_out, error);
        if (error)
            return (error);
        *fd_in = open(filename, O_RDONLY);
        if (*fd_in == -1)
            return(perror("open"), 1);
        *node = (*node)->child;
    }
    return (0);
}
*/

char *handle_here_doc(t_parse_tree **node, t_exec_vars *vars)
{
    char *buffer;
    char *filename = "/tmp/heredoc.txt";
    int file = open(filename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR); //CHECK HOW TO REPLACE FOPEN WITH OPEN PROPERLY
    if (file == -1)
    {
        vars->error = 1;
        return (perror("open"), NULL);
    }
    while ((buffer = readline("heredoc> ")) != NULL)
    {
        if (strcmp(buffer, (*node)->child->data->lexeme) == 0)
            break;
        write(file, buffer, strlen(buffer)); // Write to file
        write(file, "\n", 1); // Write newline character
        free(buffer);
    }
    close(file);
    return (filename);
}
/*
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
*/

int exec_builtins(char **args, t_env **env, char **environment)
{
    if (strcmp(args[0], "cd") == 0)
        return(exec_cd(args, env));
    else if (strcmp(args[0], "pwd") == 0)
        return(exec_pwd(args));
    else if (strcmp(args[0], "echo") == 0)
        return(exec_echo(args), 0);
    else if (strcmp(args[0], "export") == 0)
        return(exec_export(args, env));
    else if (strcmp(args[0], "unset") == 0)
        return(exec_unset(args, env));
    else if (strcmp(args[0], "env") == 0)
        return(exec_env(args, env, environment));
    return (2);
}

int handle_child_cmd(t_exec_vars *vars, t_env **env, char **environment)
{
    char *path;
    if (vars->fd_in != 0)
    {
        dup2(vars->fd_in, 0);
        close(vars->fd_in);
    }
    if (vars->fd_out != 1)
    {
        dup2(vars->fd_out, 1);
        close(vars->fd_out);
    }
    if (get_path(vars->args[0], *env, &path))
        return 1;
    if (execve(path, vars->args, environment) < 0)
    {
        perror("execve");
        exit(EXIT_FAILURE);
    }
    return 0;
}

int handle_fork(t_exec_vars *vars, t_env **env, char **environment)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        if (handle_child_cmd(vars, env, environment))
            return 1;
    }
    else // Parent process
        waitpid(pid, &status, 0);

    return 0;
}

int execute_command(t_exec_vars *vars,  t_env **env)
{
    char **environment;
    int return_builtins;

    environment = env_list_to_array(*env);
    if (environment == NULL)
        return 1;
    return_builtins = exec_builtins(vars->args, env, environment);
    if (return_builtins == 2)
    {
        if (handle_fork(vars, env, environment))
            return (free_env_array(environment), 1);
    }
    else if (return_builtins == 1)
        return (free_env_array(environment), 1);
    free_env_array(environment);
    return 0;
}

void init_exec_vars(t_exec_vars *vars)
{
    vars->fd_in = 0;
    vars->fd_out = 1;
    int i = 0;
    while (i < 10)
        vars->args[i++] = NULL;
    vars->i = 0;
    vars->error = 0;
}

void handle_node_data(t_parse_tree *node, t_exec_vars *vars, t_env **env)
{
    if (node->data->type == RED_FROM || node->data->type == RED_TO
        || node->data->type == APPEND || node->data->type == HERE_DOC)
        handle_redirection(&node, vars);
    else if (node->data->lexeme[0] == '$')
        handle_global_env(node, vars->args, vars->i++, env);
    else if (node->data->lexeme[0] == '"')
        handle_quotes_global(node, vars->args, vars->i++, env);
    else
        vars->args[vars->i++] = node->data->lexeme;
}

int execute_node(t_parse_tree *node, t_env **env)
{
    t_exec_vars vars;
    init_exec_vars (&vars);

    if (node == NULL)
        return (0);
    while (node != NULL)
    {
        if (node->data != NULL)
        {
            handle_node_data(node, &vars, env);
            if (vars.error == 1)
                return (vars.error);
        }
        node = node->child;
    }
    vars.args[vars.i] = NULL;
    if (execute_command(&vars, env) == 1)
        return (1);
    return (0);
}

void handle_global_env(t_parse_tree *node, char **args, int i, t_env **env)
{
    char *env_var_name = node->data->lexeme + 1;
    char *env_var_value = get_env_var(*env, env_var_name);
    if (env_var_value != NULL)
        args[i] = env_var_value;
    else
        args[i] = "";
}


void handle_quotes_global(t_parse_tree *node, char **args, int i, t_env **env)
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
        char *var_value = get_env_var(*env, var_name);
        if (var_value != NULL)
            strcat(buffer, var_value);
        start = var_end;
    }
    args[i] = ft_strdup(buffer);
}

int execute_parse_tree(t_parse_tree *root, t_env **env)

{
    if (root == NULL)
        return 0; //NO IMPUT, NOT A MISTAKE
    if (root->sibling)
    {
        if (execute_pipeline(root, env))
            return (1);
    }
    else
    {
        if (execute_node(root->child, env))
            return (1);
    }
    return (0);
}

int handle_child_process(t_parse_tree *node, t_env **env, int *pipefd)
{
    if (node->sibling != NULL)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
    }
    if (execute_node(node, env) != 0)
        exit(EXIT_FAILURE);
    exit(EXIT_SUCCESS);
}

int handle_sibling_process(t_parse_tree *node, t_env **env, int *pipefd)
{
    pid_t pid2 = fork();
    if (pid2 == 0)
    {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        if(execute_pipeline(node->sibling->sibling, env))
            exit(EXIT_FAILURE);
        exit(EXIT_SUCCESS);
    }
    else if (pid2 > 0)
    {
        close(pipefd[0]);
        wait(NULL);
    }
    else
        return (perror("fork"), 1);
    return 0;
}

int handle_parent_process(t_parse_tree *node, t_env **env, int *pipefd, pid_t pid)
{
    int status;

    waitpid(pid, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) != EXIT_SUCCESS)
        return (perror("Child process failed"), 1);
    if (node->sibling != NULL)
    {
        close(pipefd[1]);
        if (handle_sibling_process(node, env, pipefd))
            return 1;
    }
    wait(NULL);
    return 0;
}

int execute_pipeline(t_parse_tree *node, t_env **env)
{
    int pipefd[2];
    pid_t pid;

    if (node == NULL)
        return (0);
    if (node->sibling != NULL)
    {
        if (pipe(pipefd) == -1)
            return (perror("pipe"), 1);
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return 1;
    }
    else if (pid == 0)
        return (handle_child_process(node, env, pipefd));
    else
        return (handle_parent_process(node, env, pipefd, pid));
}

int update_pwd(t_env **env, char *cwd)
{
    if (update_add_env_var(env, "OLDPWD", cwd))
        return (1);
    cwd = getcwd(NULL, 0);  // Get the current working directory again
    if (cwd == NULL)
        return (perror("getcwd"), 1);
    if (update_add_env_var(env, "PWD", cwd))
        return (free(cwd), 1);
    return (free(cwd), 0);;  // Free the current working directory string
}

int change_directory_and_update(char *path, t_env **env, char *cwd)
{
    if (chdir(path) != 0)
    {
        perror("chdir");
        free(cwd);
        return 1;
    }
    if (update_pwd(env, cwd))
    {
        free(cwd);
        return 1;
    }
    free(cwd);
    return 0;
}

int exec_cd(char **args, t_env **env)
{
    char *cwd;

    cwd = getcwd(NULL, 0);
    if (cwd == NULL)
        return (perror("getcwd"), 1);
    if (args[1] != NULL && args[2])
        return (perror("cd: too many arguments\n"), free(cwd), 1);
    else if (args[1] == NULL || strcmp(args[1], "~") == 0)
        return change_directory_and_update(get_env_var(*env, "HOME"), env, cwd);
    else if (strcmp(args[1], "..") == 0)
        return change_directory_and_update("..", env, cwd);
    else
        return change_directory_and_update(args[1], env, cwd);
}



void    exec_echo(char **args)//CHANGE IT TO FT_PRINTF AND FT_LIBFT
{
    int i = 1;
    if (args[i] == NULL)
    {
        printf("\n");
        return ;
    }
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

int    exec_pwd(char **args)
{
    char    *cwd;

    cwd = getcwd(NULL, 0);
    if (cwd == NULL)
        return (perror("getcwd"), 1);
    else
    {
        printf("%s\n", cwd);
        free(cwd);
        return (0);
    }
}

int    exec_env(char **args, t_env **env, char **environment)
{
    //char **environtment;
    int i;

    i = 0;
    if (args[1] != NULL)
        return (perror("env: too many arguments\n"), 1);
    //environtment = env_list_to_array(*env);
    //if (environtment == NULL)
     //   return (1);
    while (environment[i] != NULL)
        printf("%s\n", environment[i++]);
    //free_env_array(environment);
    return (0);
}

int    exec_unset (char **args, t_env **env)
{
    t_env *current;
    t_env *prev;

    if (args[2] != NULL)
        return(perror ("unset: too many arguments\n"), 1);

    current = *env;
    prev = NULL;
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
            return (0);
        }
        prev = current;
        current = current->next;
    }
    return (0);//IF NOT FOUND, IT IS NOT AN ERROR?
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
        return (perror("export: not a valid identifier\n"), 1);
    while (args[i])
    {
        if(args[i++] == '=') //Seems like in bash it is not a problem not to have =
            return (2);
    }
    return(0);
}

int split_var(char *var, char **name, char **value)
{
    char *equals = strchr(var, '=');
    *name = strndup(var, equals - var);
    if (name == NULL)
        return(perror("split_var: strndup error\n"), 1);
    *value = strdup(equals + 1);
    if (value == NULL)
        return(perror("split_var: strndup error\n"), free(*name), 1);
    return (0);
}

int    exec_export(char **args, t_env **env)
{
    char *name;
    char *value;
    int control;

    name = NULL;
    value = NULL;
    if (args[1] == NULL)
        return(exec_export_no_args(*env), 0);
    control = var_control(args[1]);
    if (control == 1)
        return (1);
    if (control == 2)
    {
        if (split_var(args[1], &name, &value))//think how to handle the error
            return (1);
        if (update_add_env_var(env, name ,value))
            return(free(name), free(value), 1);
    }
    return (0);
}
