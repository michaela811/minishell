/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_handle_heredoc.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:15 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/06 16:17:52 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	file_error(int file, t_exec_vars *vars)
{
	if (file == -1)
	{
		vars->error = 1;
		return (1);
	}
	return (0);
}

int	malloc_check(char *no_quotes_lex, int *error)
{
	if (no_quotes_lex == NULL)
	{
		*error = 1;
		print_err(1, 2, "my(s)hell: malloc\n");
		return (1);
	}
	return (0);
}

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
	file = open_heredoc_file(filename, vars);
	if (file_error(file, vars))
		return (free(no_quotes_lex), NULL);
	if (ft_strpbrk((*node)->child->data->lexeme, "'\"") != NULL)
	{
		remove_quotes(&no_quotes_lex, &vars->error);
		if (vars->error)
			return (free(no_quotes_lex), close(file), NULL);
		if (process_heredoc_dollar_closed(file, no_quotes_lex))
			vars->error = 1;
	}
	else if (process_heredoc_dollar_open(file, vars, exec_data, no_quotes_lex))
		vars->error = 1;
	close(file);
	free(no_quotes_lex);
	return (filename);
}

int	open_heredoc_file(char *filename, t_exec_vars *vars)
{
	int	file;

	file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (file == -1)
	{
		vars->error = 1;
		print_err(1, 2, "my(s)hell: open error\n");
	}
	return (file);
}
