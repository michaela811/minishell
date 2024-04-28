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
        if (ft_strcmp(buffer, (*node)->child->data->lexeme) == 0)
            break;
        write(file, buffer, ft_strlen(buffer)); // Write to file
        write(file, "\n", 1); // Write newline character
        free(buffer);
    }
    close(file);
    return (filename);
}
