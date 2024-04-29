#include "lexer.h"

void	init_exec_vars(t_exec_vars *vars)
{
	int	i;

	vars->fd_in = 0;
	vars->fd_out = 1;
	i = 0;
	while (i < 10)
		vars->args[i++] = NULL;
	vars->i = 0;
	vars->error = 0;
}

void	handle_node_data(t_parse_tree *node, t_exec_vars *vars, t_env **env)
{
	if (node->data->type == RED_FROM || node->data->type == RED_TO
		|| node->data->type == APPEND || node->data->type == HERE_DOC)
		handle_redirection(&node, vars);
	else if (node->data->lexeme[0] == '$'
		&& strcmp(node->data->lexeme, "$?") == 0)
		vars->args[vars->i++] = "$?";
	else if (node->data->lexeme[0] == '$')
		handle_global_env(node, vars->args, vars->i++, env);
	else if (node->data->lexeme[0] == '"' || node->data->lexeme[0] == 39)
		handle_quotes_global(node, vars->args, vars->i++, env);
	else
	{
		remove_even_quotes(node, vars->args, vars->i++, vars->error);
		//vars->args[vars->i++] = node->data->lexeme;
	}
}

void 	remove_even_quotes(t_parse_tree *node, char **args, int i, int error)
{
	int		quote_count;
	char	*str;
	int		k;
	char	*new_str;
	int		j;

	quote_count = 0;
	str = node->data->lexeme;
	k = 0;
	while (str[k] != '\0')
    {
        if (str[k] == '"' || str[k] == '\'')
            quote_count++;
		k++;
    }
    if (quote_count % 2 != 0)
	{
		error = 1;
        return ;
	}
    new_str = malloc(ft_strlen(str) + 1);
    j = 0;
	k = 0;
    while (str[k] != '\0')
    {
        if (str[k] != '"' && str[k] != '\'')
            new_str[j++] = str[k];
		k++;
    }
    new_str[j] = '\0';
    args[i] = new_str;
}

void	handle_redirection(t_parse_tree **node, t_exec_vars *vars)
{
	if ((*node)->data->type == RED_FROM)
		return (handle_redirection_from(node, vars));
	else if ((*node)->data->type == RED_TO)
		return (handle_redirection_to(node, vars));
	else if ((*node)->data->type == APPEND)
		return (handle_redirection_append(node, vars));
	else if ((*node)->data->type == HERE_DOC)
		return (handle_redirection_here_doc(node, vars));
}
