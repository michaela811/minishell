#include "minishell.h"



void	handle_redirection_from(t_parse_tree **node, t_exec_vars *vars, t_env **env)
{
	handle_quotes_glob_redirect(node, vars, env);
	if (vars->error)
		return ;
	vars->fd_in = open((*node)->child->data->lexeme, O_RDONLY);
	if (vars->fd_in == -1)
	{
		vars->error = 1;
		printf_global_error(1, 2, "my(s)hell: %s: No such file or directory\n", (*node)->child->data->lexeme);
	}
	*node = (*node)->child;
}

void	handle_redirection_to(t_parse_tree **node, t_exec_vars *vars, t_env **env)
{
	handle_quotes_glob_redirect(node, vars, env);
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

void	handle_redirection_append(t_parse_tree **node, t_exec_vars *vars, t_env **env)
{
	int is_dir;
	char *start;
	char *expanded_lexeme;

	handle_quotes_glob_redirect(node, vars, env);
	if (g_last_exit_status)
		return ;
	expanded_lexeme = malloc(4096);
    if (!expanded_lexeme)
	{
        perror("malloc");
        vars->error = 1;
        return;
    }
	ft_memset(expanded_lexeme, '\0', sizeof(expanded_lexeme));
    start = (*node)->child->data->lexeme;
    handle_dollar_sign(&start, expanded_lexeme, env, sizeof(expanded_lexeme));
	is_dir = is_directory(expanded_lexeme);
	if (is_dir == 1)
	{
        vars->error = 1;
        printf_global_error(1, 2, "my(s)hell: %s: Is a directory\n", expanded_lexeme);
        free(expanded_lexeme);
        return;
	}
	free(expanded_lexeme);
	vars->fd_out = open((*node)->child->data->lexeme, O_WRONLY | O_CREAT | O_APPEND, 0644);
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

void	handle_redirection_here_doc(t_parse_tree **node, t_exec_vars *vars)//, t_env **env)
{
	char	*filename;

	filename = handle_here_doc(node, vars);//, env);
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

char	*handle_here_doc(t_parse_tree **node, t_exec_vars *vars)//, t_env **env)
{
	char	*buffer;
	char	*filename;
	int		file;
	char	*line;

	filename = "/tmp/heredoc.txt";
	(*node)->child->data->lexeme = handle_quotes_echo((*node)->child->data->lexeme, &vars->error);
	//handle_quotes_glob(&(*node)->child->data->lexeme, env, &vars->error);
	file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (file == -1)
	{
		vars->error = 1;
		return (perror("open"), NULL);
	}
	while (1)
	{
		/*buffer = readline("heredoc> ");
		if (buffer == NULL)
			break ;*/
		line = get_next_line(fileno(stdin)); // for tester from this line
		if (line == NULL)
            break;
        buffer = ft_strtrim(line, "\n");
		free(line); // to this line
		if (ft_strcmp(buffer, (*node)->child->data->lexeme) == 0)
		{
			free(buffer);
			break ;
		}
		write(file, buffer, ft_strlen(buffer));
		write(file, "\n", 1);
		free(buffer);
	}
	close(file);
	return (filename);
}
