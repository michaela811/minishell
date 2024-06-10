#include "minishell.h"

void	handle_redirection_from(t_parse_tree **node, t_exec_vars *vars)
{
	(*node)->child->data->lexeme = handle_quotes_echo((*node)->child->data->lexeme, &vars->error);
	if (vars->error)//maybe better vars->error
		return ;
	vars->fd_in = open((*node)->child->data->lexeme, O_RDONLY);
	if (vars->fd_in == -1)
	{
		vars->error = 1;
		printf_global_error(1, 2, "my(s)hell: %s: No such file or directory\n", (*node)->child->data->lexeme);
	}
	*node = (*node)->child;
	//vars->i++;
}

void	handle_redirection_to(t_parse_tree **node, t_exec_vars *vars)
{
	(*node)->child->data->lexeme = handle_quotes_echo((*node)->child->data->lexeme, &vars->error);
	if (g_last_exit_status)
		return ;
	vars->fd_out = open((*node)->child->data->lexeme,
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (vars->fd_out == -1)
	{
		vars->error = 1;
		if (errno == EACCES)
        	printf_global_error(1, 2, "my(s)hell: %s: Permission denied\n", (*node)->child->data->lexeme);
    	else
        printf_global_error(1, 2, "my(s)hell: %s: No such file or directory\n", (*node)->child->data->lexeme);
	}
	*node = (*node)->child;
	vars->i++;
}

void	handle_redirection_append(t_parse_tree **node, t_exec_vars *vars)
{
	(*node)->child->data->lexeme = handle_quotes_echo((*node)->child->data->lexeme, &vars->error);
	if (g_last_exit_status)
		return ;
	vars->fd_out = open((*node)->child->data->lexeme,
			O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (vars->fd_out == -1)
	{
		vars->error = 1;
		if (errno == EACCES)
        	printf_global_error(1, 2, "my(s)hell: %s: Permission denied\n", (*node)->child->data->lexeme);
    	else
        printf_global_error(1, 2, "my(s)hell: %s: No such file or directory\n", (*node)->child->data->lexeme);
	}
	*node = (*node)->child;
	vars->i++;
}

void	handle_redirection_here_doc(t_parse_tree **node, t_exec_vars *vars)
{
	char	*filename;

	filename = handle_here_doc(node, vars);
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

char	*handle_here_doc(t_parse_tree **node, t_exec_vars *vars)
{
	char	*buffer;
	char	*filename;
	int		file;
	/*int		expand_vars;
	char	*delimiter;
	char	*expanded_buffer;*/

	filename = "/tmp/heredoc.txt";
	file = open(filename, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if (file == -1)
	{
		vars->error = 1;
		return (perror("open"), NULL);
	}
	//delimiter = (*node)->child->data->lexeme;
    //expand_vars = !(delimiter[0] == '\'' || delimiter[0] == '\"');
	while (1)
	{
		buffer = readline("heredoc> ");
		if (buffer == NULL)
			break ;
		if (ft_strcmp(buffer, (*node)->child->data->lexeme) == 0)
		{
			free(buffer);
			break ;
		}
		/*if (expand_vars)
		{
			expanded_buffer = expand_variables(buffer, vars->env);
			write(file, expanded_buffer, ft_strlen(expanded_buffer));
			free(expanded_buffer);
		}*/
		write(file, buffer, ft_strlen(buffer));
		write(file, "\n", 1);
		free(buffer);
	}
	close(file);
	return (filename);
}
