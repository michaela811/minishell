#include "lexer.h"

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
    else if (node->data->lexeme[0] == '$' && strcmp(node->data->lexeme, "$?") == 0)
        vars->args[vars->i++] = "$?";
    else if (node->data->lexeme[0] == '$')
        handle_global_env(node, vars->args, vars->i++, env);
    else if (node->data->lexeme[0] == '"')
        handle_quotes_global(node, vars->args, vars->i++, env);
    else
        vars->args[vars->i++] = node->data->lexeme;
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
