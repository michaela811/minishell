/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_here_doc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:21 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/17 16:10:12 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_heredoc(t_p_tree **node, t_exec_vars *vars, t_hd_data *here_docs)
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
	here_docs->fd = fd;
	vars->i++;
	unlink(filename);
	return (0);
}

static int	get_stdin(void)
{
	int				original_stdin;
	int				tty_fd;
	struct termios	term;

	original_stdin = dup(STDIN_FILENO);
	if (original_stdin == -1)
	{
		perror("my(s)hell: dup");
		return (1);
	}
	if (tcgetattr(STDIN_FILENO, &term) == -1)
	{
		tty_fd = open("/dev/tty", O_RDONLY);
		if (tty_fd == -1)
			return (perror("my(s)hell: open /dev/tty"), close(original_stdin),
				1);
		if (dup2(tty_fd, STDIN_FILENO) == -1)
			return (perror("my(s)hell: dup2 tty_fd to STDIN_FILENO"),
				close(tty_fd), close(original_stdin), 1);
		close(tty_fd);
	}
	return (0);
}

char	*get_heredoc_content(char *contents, char *buffer)
{
	char	*temp;

	temp = NULL;
	if (contents == NULL)
	{
		contents = malloc(ft_strlen(buffer) + 2);
		if (contents == NULL)
			return (free(buffer), NULL);
		ft_strcpy(contents, buffer);
	}
	else
	{
		temp = realloc(contents, ft_strlen(contents) + ft_strlen(buffer) + 2);
		if (temp == NULL)
			return (free(contents), free(buffer), NULL);
		contents = temp;
		ft_strcat(contents, buffer);
	}
	return (contents);
}

static int	is_it_delimiter(t_p_tree **node, char *buffer)
{
	if (ft_exact_strcmp(buffer, (*node)->data->lexeme) == 0)
	{
		free(buffer);
		return (1);
	}
	return (0);
}
int	pipe_get_heredoc(t_p_tree **node, t_exec_vars *vars, int fd)
{
	char			*buffer;
	char			*line;
	char			*contents;

	contents = NULL;
	(*node)->data->lexeme = handle_quotes_echo
		((*node)->data->lexeme, &vars->error);
	if (get_stdin())
		return (1);
	while (1)
	{
		line = get_next_line(fileno(stdin));
		if (line == NULL)
			break ;
		buffer = ft_strtrim(line, "\n");
		free(line);
		if (is_it_delimiter(node, buffer))
			break ;
		contents = get_heredoc_content(contents, buffer);
		ft_strcat(contents, "\n");
		free(buffer);
	}
	if (contents != NULL)
	{
		write(fd, contents, strlen(contents));
		free(contents);
	}
	return (0);
} //for tester

/* FOR EVALUATION
int	pipe_get_heredoc(t_p_tree **node, t_exec_vars *vars, int fd)
{
	char			*buffer;
	char			*contents;

	contents = NULL;
	(*node)->data->lexeme = handle_quotes_echo
		((*node)->data->lexeme, &vars->error);
	if (get_stdin())
		return (1);
	while (1)
	{
		buffer = readline("heredoc> ");
		if (buffer == NULL)
			break ;
		if (is_it_delimiter(node, buffer))
			break ;
		contents = get_heredoc_content(contents, buffer);
		ft_strcat(contents, "\n");
		free(buffer);
	}
	if (contents != NULL)
	{
		write(fd, contents, strlen(contents));
		free(contents);
	}
	return (0);
}*/

void	init_heredocs(t_hd_data **here_docs)
{
	if (*here_docs == NULL)
	{
		*here_docs = malloc(sizeof(t_hd_data));
		if (*here_docs == NULL)
		{
			perror("my(s)hell: malloc failed");
			exit(1);
		}
	}
	else if ((*here_docs)->fd != -1)
		close((*here_docs)->fd);
	(*here_docs)->fd = -1;
}

int	is_there_here_doc(t_p_tree **tree, t_hd_data **here_docs)
{
	t_p_tree	*current;
	t_exec_vars	*vars;

	current = *tree;
	vars = malloc(sizeof(t_exec_vars));
	if (!vars)
		return (print_err(1, 2, "my(s)hell: execute_node malloc error\n"), 1);
	init_exec_vars(vars);
	if (current != NULL)
	{
		while (current->child != NULL)
		{
			if (current->child->data != NULL
				&& current->child->data->type == HERE_DOC)
			{
				init_heredocs(here_docs);
				if (pipe_heredoc(&current->child->child, vars, *here_docs) == 0)
					return (free(vars), 0);
			}
			else
				current = current->child;
		}
	}
	free(vars);
	return (1);
}
