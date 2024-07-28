/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_here_doc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:21 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/24 14:13:02 by mmasarov         ###   ########.fr       */
/*   Updated: 2024/07/18 13:32:05 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_stdin(void)
{
	int				original_stdin;
	int				tty_fd;
	struct termios	term;

	original_stdin = dup(STDIN_FILENO);
	if (original_stdin == -1)
	{
		print_err(errno, 2, "my(s)hell: dup");
		return (1);
	}
	if (tcgetattr(STDIN_FILENO, &term) == -1)
	{
		tty_fd = open("/dev/tty", O_RDONLY);
		if (dup2(tty_fd, STDIN_FILENO) == -1)
			return (print_err(errno, 2,
					"my(s)hell: dup2 tty_fd to STDIN_FILENO"),
				close(tty_fd), close(original_stdin), 1);
		close(tty_fd);
	}
	return (0);
}

char	*get_heredoc_content(char *contents, char *buffer)
{
	char	*temp;
	size_t	old_size;
	size_t	new_size;

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
		old_size = ft_strlen(contents) + 1;
		new_size = old_size + ft_strlen(buffer) + 1;
		temp = ft_realloc(contents, old_size, new_size);
		//temp = realloc(contents, ft_strlen(contents) + ft_strlen(buffer) + 2);
		if (temp == NULL)
			return (free(contents), free(buffer), NULL);
		contents = temp;
		ft_strcat(contents, buffer);
	}
	return (contents);
}

static int	is_it_delimiter(char *node, char *buffer)
{
	if (ft_exact_strcmp(buffer, node) == 0)
	{
		free(buffer);
		return (1);
	}
	return (0);
}

/*int	pipe_get_heredoc(t_p_tree **node, t_exec_vars *vars, int fd)
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
} //for tester*/

 //FOR EVALUATION
int	pipe_get_heredoc(t_p_tree **node, t_exec_vars *vars, int fd)
{
	char			*buffer;
	char			*contents;
	char			*dup_lexeme;

	contents = NULL;
	//dup_lexeme = NULL;
	dup_lexeme = handle_quotes_heredoc((*node)->data->lexeme, &vars->error);
	if (get_stdin())
		return (1);
	while (1)
	{
		buffer = readline("heredoc> ");
		if (buffer == NULL)
			break ;
		if (is_it_delimiter(dup_lexeme, buffer))
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
	return (free(dup_lexeme), 0);
}
