/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_here_doc_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:15:59 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/03 18:20:18 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_heredoc_dollar_open(char *lexeme_no_quotes, t_exec_vars *vars, int fd, t_env **env)
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
		buffer = readline("heredoc> ");
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
     char *lexeme_no_quotes;

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
    {
        vars->error = 1;
    }
    free(lexeme_no_quotes);
}

int	pipe_heredoc(t_p_tree **node, t_exec_vars *vars, int *here_docs, t_env **env)
{
	char	*filename;
	//char	*lexeme_no_quotes;
	int		fd;

	filename = "/tmp/heredoc.txt";
	//lexeme_no_quotes = ft_strdup((*node)->child->data->lexeme);
	//if (lexeme_no_quotes == NULL)
		//return (1);
	fd = open_heredoc_file(filename, vars);
	if (fd == -1)
		return 1;//(free(lexeme_no_quotes), 1);
	//pipe_get_heredoc1(node, vars, fd, env);
	handle_heredoc_quotes(fd, node, vars, env);
	/* if (ft_strpbrk((*node)->child->data->lexeme, "'\"") != NULL)
	{
		remove_quotes(&lexeme_no_quotes, &vars->error);
		if (!vars->error)
			//return (free(lexeme_no_quotes), close(fd), NULL);
			process_heredoc_dollar_closed(fd, lexeme_no_quotes);
	}
	else if (process_heredoc_dollar_open(fd, vars, env, lexeme_no_quotes))
		vars->error = 1;
	free(lexeme_no_quotes);//probably no need to free */
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

/*void	init_heredocs(t_hd_data **here_docs)
{
	if (*here_docs == NULL)
	{
		*here_docs = malloc(sizeof(t_hd_data));
		if (*here_docs == NULL)
		{
			print_err(1, 2, "my(s)hell: malloc heredoc\n");
			exit(1);
		}
	}
	else if ((*here_docs)->fd != -1)
		close((*here_docs)->fd);
	(*here_docs)->fd = -1;
}*/

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
