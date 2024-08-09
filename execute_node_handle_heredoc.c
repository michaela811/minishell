/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_handle_heredoc.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:15 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/09 20:50:03 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	malloc_check(char *no_quotes_lex, int *error)
{
	if (no_quotes_lex == NULL)
	{
		*error = 1;
		print_err(1, 2, "my(s)hell: malloc error 2\n");
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

int	open_heredoc_file(char *filename)
{
	int	file;

	file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (file == -1)
		print_err(1, 2, "my(s)hell: open error\n");
	return (file);
}
