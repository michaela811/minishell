/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_handle_heredoc.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:15 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/10 13:56:13 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_here_doc(t_p_tree **node, t_exec_vars *vars,
		t_free_data *exec_data)
{
	char	*filename;
	char	*no_quotes_lex;
	int		file;

	filename = "/tmp/heredoc.txt";
	no_quotes_lex = ft_strdup((*node)->child->data->lexeme);
	if (malloc_check(no_quotes_lex, &vars->error))
		return (NULL);
	file = open_heredoc_file(filename);
	if (file == -1)
		return (free(no_quotes_lex), NULL);
	if (ft_strpbrk((*node)->child->data->lexeme, "'\"") != NULL)
	{
		remove_quotes(&no_quotes_lex);
		if (g_last_exit_status)
			return (free(no_quotes_lex), close(file), NULL);
		process_heredoc_dollar_closed(file, no_quotes_lex);
	}
	else
		process_heredoc_dollar_open(file, vars, exec_data, no_quotes_lex);
	close(file);
	free(no_quotes_lex);
	return (filename);
}

int	control_hd(char *rl_input, t_handle_vars *l_vars,
	char *no_quotes_lex)
{
	if (break_heredoc(rl_input))
		return (free(l_vars->buffer), 1);
	if (rl_input == NULL)
		return (ft_printf_fd(STDOUT_FILENO,
				"my(s)hell: heredoc delimited by EOF\n"),
			free(l_vars->buffer), 0);
	if (ft_exact_strcmp(rl_input, no_quotes_lex) == 0)
		return (free(rl_input), free(l_vars->buffer), 0);
	while ((int)ft_strlen(rl_input) >= l_vars->buffer_size)
		if (resize_buffer(&l_vars->buffer, &l_vars->buffer_size))
			return (free(l_vars->buffer), free(rl_input), 1);
	return (-1);
}

int	process_heredoc_dollar_open(int file, t_exec_vars *vars,
		t_free_data *exec_data, char *no_quotes_lex)
{
	t_handle_vars	l_vars;
	char			*rl_input;
	char			*buffer_start;
	int				control_result;

	rl_input = NULL;
	l_vars.buffer_size = 4096;
	l_vars.buffer = malloc(l_vars.buffer_size);
	if (l_vars.buffer == NULL)
		return (print_err(1, 2, "my(s)hell: malloc error x1"), 1);
	while (1)
	{
		rl_input = readline("heredoc> ");
		control_result = control_hd(rl_input, &l_vars, no_quotes_lex);
		if (control_result != -1)
			return (control_result);
		buffer_start = rl_input;
		if (handle_dollar_error(&rl_input, &l_vars, vars, exec_data))
			return (free(buffer_start), free(l_vars.buffer), 1);
		write(file, l_vars.buffer, ft_strlen(l_vars.buffer));
		write(file, "\n", 1);
		free(buffer_start);
	}
	return (0);
}

int	process_heredoc_dollar_closed(int file, char *no_quotes_lex)
{
	t_handle_vars	l_vars;
	char			*rl_input;

	l_vars.buffer_size = 4096;
	l_vars.buffer = malloc(l_vars.buffer_size);
	if (l_vars.buffer == NULL)
		return (print_err(1, 2, "my(s)hell: malloc error x1"), 1);
	while (1)
	{
		rl_input = readline("heredoc> ");
		if (break_heredoc(rl_input))
			return (free(l_vars.buffer), 1);
		if (rl_input == NULL)
			return (ft_printf_fd(STDOUT_FILENO, "my(s)hell: heredoc delimited"
					" by EOF\n"), free(l_vars.buffer), 0);
		if (ft_exact_strcmp(rl_input, no_quotes_lex) == 0)
			return (free(rl_input), free(l_vars.buffer), 0);
		while ((int)ft_strlen(rl_input) >= l_vars.buffer_size)
			if (resize_buffer(&l_vars.buffer, &l_vars.buffer_size))
				return (free(l_vars.buffer), free(rl_input), 1);
		write(file, rl_input, ft_strlen(rl_input));
		write(file, "\n", 1);
		free(rl_input);
	}
	return (free(l_vars.buffer), 0);
}

int	open_heredoc_file(char *filename)
{
	int	file;

	file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (file == -1)
		print_err(1, 2, "my(s)hell: open error\n");
	return (file);
}
