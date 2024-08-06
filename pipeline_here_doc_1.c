/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_here_doc_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:15:59 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/06 18:08:23 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	break_pipe_heredoc(char *buffer, char *contents)
{
	if (g_last_exit_status == 130)
	{
		if (contents != NULL)
			free(contents);
		free(buffer);
		return (1);
	}
	return (0);
}

int	pipe_heredoc_dollar_open(char *no_quotes_lex, t_exec_vars *vars,
		int fd, t_free_data *exec_data)
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
		if (break_pipe_heredoc(buffer, contents))
			return (1);
		if (buffer == NULL)
			break ;
		if (is_it_delimiter(no_quotes_lex, buffer))
			break ;
		buffer_start = buffer;
		if (handle_dollar_error(&buffer, buffer_no_dollar, vars, exec_data))
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

int	pipe_heredoc_dollar_closed(char *no_quotes_lex, t_exec_vars *vars, int fd)
{
	char	*buffer;
	char	*contents;

	contents = NULL;
	if (get_stdin())
		return (1);
	while (1)
	{
		buffer = readline("heredoc> ");
		if (break_pipe_heredoc(buffer, contents))
			return (1);
		if (buffer == NULL)
		{
			vars->error = 1;
			break ;
		}
		if (is_it_delimiter(no_quotes_lex, buffer))
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

void handle_heredoc_quotes(int fd, t_p_tree **node, t_exec_vars *vars,
		t_free_data *exec_data)
{
	char	*no_quotes_lex;

	no_quotes_lex = ft_strdup((*node)->data->lexeme);
	if (no_quotes_lex == NULL)
	{
		vars->error = 1;
		return;
	}
	if (ft_strpbrk((*node)->data->lexeme, "'\"") != NULL)
	{
		remove_quotes(&no_quotes_lex, &vars->error);
		if (!vars->error)
			if (pipe_heredoc_dollar_closed(no_quotes_lex, vars, fd))
				vars->error = 1;
	}
	else if (pipe_heredoc_dollar_open(no_quotes_lex, vars, fd, exec_data))
		vars->error = 1;
	free(no_quotes_lex);
}

int	pipe_heredoc(t_p_tree **node, t_exec_vars *vars, int *here_docs,
		t_free_data *exec_data)
{
	char	*filename;
	int		fd;

	filename = "/tmp/heredoc.txt";
	fd = open_heredoc_file(filename, vars);
	if (fd == -1)
		return 1;
	handle_heredoc_quotes(fd, node, vars, exec_data);
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

int handle_pipe_heredoc(t_exec_vars **vars, int *here_docs,
		t_p_tree *current, t_free_data *exec_data)
{
	if (*here_docs != -1)
	{
		free_array((*vars)->args);
		free(*vars);
		*vars = NULL;
		close(*here_docs);
		*here_docs = -1;
	}
	*vars = malloc(sizeof(t_exec_vars));
	if (!*vars)
		return (print_err(1, 2, "my(s)hell: execute_node malloc error\n"), 1);
	init_exec_vars(*vars);
	pipe_heredoc(&current->child->child, *vars, here_docs, exec_data);
	if (g_last_exit_status == 130)
	{
		free_array((*vars)->args);
		free(*vars);
		return (1);
	}
	return (0);
}

int	is_there_here_doc(t_p_tree **tree, int *here_docs, t_free_data *exec_data)
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
				if (handle_pipe_heredoc(&vars, here_docs, current, exec_data))
					return (1);
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
