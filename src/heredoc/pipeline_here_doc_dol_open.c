/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_here_doc_dol_open.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 17:03:19 by dpadenko          #+#    #+#             */
/*   Updated: 2024/08/10 20:00:07 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_buffer(char *buffer, t_handle_vars *l_vars, t_free_data *exec_data)
{
	if (handle_dollar_sign(&buffer, l_vars, exec_data))
		return (1);
	return (0);
}

int	pipe_heredoc_dollar_open(char *no_quotes_lex, int fd,
		t_free_data *exec_data)
{
	char			*rl_input;
	t_handle_vars	l_vars;
	char			*buffer_start;
	char			*contents;
	int				control_result;

	if (initialize_vars_pipe_hd(&rl_input, &contents, &l_vars) != 0)
		return (1);
	if (get_stdin())
		return (1);
	while (1)
	{
		rl_input = readline("heredoc> ");
		control_result = check_hd_p(rl_input, &l_vars, no_quotes_lex, contents);
		if (control_result == 0)
			break ;
		else if (control_result == 1)
			return (control_result);
		buffer_start = rl_input;
		if (process_buffer(rl_input, &l_vars, exec_data))
			return (free_bufs_contents(contents, l_vars.buffer, rl_input), 1);
		if (pipe_heredoc_get_content(&contents, l_vars.buffer, buffer_start))
			return (free(l_vars.buffer), 1);
	}
	return (write_and_free_contents(fd, contents), free(l_vars.buffer), 0);
}

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

int	check_hd_p(char *rl_input, t_handle_vars *l_vars,
	char *no_quotes_lex, char *contents)
{
	if (break_pipe_heredoc(rl_input, contents))
		return (free(l_vars->buffer), 1);
	if (rl_input == NULL)
		return (ft_printf_fd(STDOUT_FILENO,
				"my(s)hell: heredoc delimited by EOF\n"), 0);
	if (ft_exact_strcmp(rl_input, no_quotes_lex) == 0)
		return (0);
	while ((int)ft_strlen(rl_input) >= l_vars->buffer_size)
		if (resize_buffer(&l_vars->buffer, &l_vars->buffer_size))
			return (free_bufs_contents(contents, l_vars->buffer, rl_input), 1);
	return (-1);
}
