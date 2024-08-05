/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_here_doc_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:15:59 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/05 18:55:00 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_sigint(int g_last_exit_status)
{
	if (g_last_exit_status == 130)
		return (1);
	return (0);
}

int	pipe_heredoc_dollar_open(char *lexeme_no_quotes, t_exec_vars *vars,
	int fd, t_env **env)
{
	char	*buffer;
	char	buffer_no_dollar[1024];
	char	*buffer_start;
	char	*contents;

	contents = NULL;
	if (get_stdin())
		return (1);
	while (1)
	{
		buffer = readline("heredoc> ");
		if (handle_sigint(g_last_exit_status))
		{
			free(buffer);
			//write(STDIN_FILENO, "\n", 1);
			//ioctl(STDOUT_FILENO, TIOCSTI, "\n");
			break ;
		}
		if (buffer == NULL)
			break ;
		if (is_it_delimiter(lexeme_no_quotes, buffer))
			break ;
		buffer_start = buffer;
		if (handle_dollar_error(&buffer, buffer_no_dollar, vars, env))
			return (free(buffer_start) , 1);
		contents = get_heredoc_content(contents, buffer_no_dollar);
		ft_strcat(contents, "\n");
		free(buffer_start);
	}
	if (contents != NULL)
	{
		write(fd, contents, ft_strlen(contents));
		free(contents);
	}
	return (0);
}

int	pipe_heredoc_dollar_closed(char *lexeme_no_quotes, t_exec_vars *vars, int fd)
{
	char	*buffer;
	char	*contents;

	contents = NULL;
	if (get_stdin())
		return (1);
	while (1)
	{
		/*if (signal(SIGINT, handle_signal))
		{
			//free(buffer);
			break ;
		}*/
		buffer = readline("heredoc> ");
		if (g_last_exit_status == 130)
		{
			free(buffer);
			break ;
		}
		if (buffer == NULL)
		{
			vars->error = 1;
			break ;
		}
		if (is_it_delimiter(lexeme_no_quotes, buffer))
			break ;
		contents = get_heredoc_content(contents, buffer);
		ft_strcat(contents, "\n");
		free(buffer);
	}
	if (contents != NULL)
	{
		write(fd, contents, ft_strlen(contents));
		free(contents);
	}
	return (0);
}

void handle_heredoc_quotes(int fd, t_p_tree **node, t_exec_vars *vars, t_env **env)
{
    char	*lexeme_no_quotes;

    lexeme_no_quotes = ft_strdup((*node)->data->lexeme);
    if (lexeme_no_quotes == NULL)
    {
        vars->error = 1;
        return;
    }
    if (ft_strpbrk((*node)->data->lexeme, "'\"") != NULL)
    {
        remove_quotes(&lexeme_no_quotes, &vars->error);
        if (!vars->error)
            pipe_heredoc_dollar_closed(lexeme_no_quotes, vars, fd);
    }
    else if (pipe_heredoc_dollar_open(lexeme_no_quotes, vars, fd, env))
        vars->error = 1;
    free(lexeme_no_quotes);
}

int	pipe_heredoc(t_p_tree **node, t_exec_vars *vars, int *here_docs, t_env **env)
{
	char	*filename;
	int		fd;

	filename = "/tmp/heredoc.txt";
	fd = open_heredoc_file(filename, vars);
	if (fd == -1)
		return 1;
	handle_heredoc_quotes(fd, node, vars, env);
	if (vars->error)
	{
		close(fd);
		unlink(filename);
		return (1);
	}
	close(fd);
	fd = open(filename, O_RDONLY);
	*here_docs = fd;
	vars->i++;
	unlink(filename);
	return (0);
}

/* int	pipe_heredoc(t_p_tree **node, t_exec_vars *vars, int *here_docs)
{
	char	*filename;
	int		fd;

	filename = "/tmp/heredoc.txt";
	fd = open_heredoc_file(filename, vars);
	pipe_get_heredoc(node, vars, fd);
	if (vars->error)
	{
		close(fd);
		unlink(filename);
		return (1);
	}
	close(fd);
	fd = open(filename, O_RDONLY);
	*here_docs = fd;
	vars->i++;
	unlink(filename);
	return (0);
} */

int	is_there_here_doc(t_p_tree **tree, int *here_docs, t_env **env)
{
	t_p_tree	*current;
	t_exec_vars	*vars = NULL;

	current = *tree;
	if (current != NULL)
	{
		while (current->child != NULL)
		{
			if (current->child->data != NULL
				&& current->child->data->type == HERE_DOC)
			{
				if (*here_docs != -1)
				{
					free_array(vars->args);
					free(vars);
					vars = NULL;
					close(*here_docs);
					*here_docs = -1;
				}
				vars = malloc(sizeof(t_exec_vars));
				if (!vars)
					return (print_err(1, 2,
						"my(s)hell: execute_node malloc error\n"), 1);
				init_exec_vars(vars);
				pipe_heredoc(&current->child->child, vars, here_docs, env);
			}
			current = current->child;
		}
	}
	if (*here_docs != -1)
	{
		free_array(vars->args);
		free(vars);
	}
	return (0);
}
