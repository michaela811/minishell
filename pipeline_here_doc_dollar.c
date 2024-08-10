/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_here_doc_dollar.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 17:03:19 by dpadenko          #+#    #+#             */
/*   Updated: 2024/08/09 20:36:39 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	break_pipe_heredoc(char *rl_input, char *contents)
{
	if (g_last_exit_status == 130)
	{
		if (contents != NULL)
			free(contents);
		free(rl_input);
		return (1);
	}
	return (0);
}

int	write_and_free_contents(int fd, char *contents)
{
	if (contents != NULL)
	{
		write(fd, contents, ft_strlen(contents));
		free(contents);
	}
	return (0);
}

void free_bufs_contents(char *contents, char *buffer, char *rl_input)
{
	if (contents != NULL)
	{
		free(contents);
		contents = NULL;
	}
	if (buffer != NULL)
	{
		free(buffer);
		buffer = NULL;
	}
	if (rl_input != NULL)
	{
		free(rl_input);
		rl_input = NULL;
	}
}

int	readline_check(char *buffer)
{
	if (buffer == NULL)
	{
		print_err(1, 2, "my(s)hell: heredoc delimited by EOF\n");
		return (1);
	}
	return (0);
}

int	process_buffer(char *buffer, t_handle_vars *l_vars, t_free_data *exec_data,
	char *buffer_start)
{
	if (handle_dollar_sign(&buffer, l_vars, exec_data))
	{
		free(buffer_start);
		return (1);
	}
	return (0);
}

int	pipe_heredoc_dollar_open(char *no_quotes_lex, int fd,
		t_free_data *exec_data)
{
	char	*rl_input;
	t_handle_vars	l_vars;
	char	*buffer_start;
	char	*contents;

	rl_input = NULL;
	contents = NULL;

	l_vars.buffer_size = 4096;
	l_vars.buffer = malloc(l_vars.buffer_size);
	if (l_vars.buffer == NULL)
		return (print_err(1, 2, "my(s)hell: malloc error x1"),
			free(l_vars.buffer), 1);
	if (get_stdin())
		return (1);
	while (1)
	{
		rl_input = readline("heredoc> ");
		if (break_pipe_heredoc(rl_input, contents))
			return (free(l_vars.buffer), 1);
		if (readline_check(rl_input))
			break ;
		if (is_it_delimiter(no_quotes_lex, rl_input))
			break ;
		while ((int)ft_strlen(rl_input) >= l_vars.buffer_size)
			if (resize_buffer(&l_vars.buffer, &l_vars.buffer_size))
				return (free_bufs_contents(contents, l_vars.buffer, rl_input), 1);
			//return (print_err(1, 2, "my(s)hell: heredoc too long\n"),
				//free (rl_input), free(l_vars.buffer), 1);
		buffer_start = rl_input;
		if (process_buffer(rl_input, &l_vars, exec_data, buffer_start))
			return (free_bufs_contents(contents, l_vars.buffer, rl_input), 1);
		if (pipe_heredoc_get_content(&contents, l_vars.buffer, buffer_start))
			return (1);
	}
	return (write_and_free_contents(fd, contents), free(l_vars.buffer), 0);
}
