/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_handle_heredoc.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:15 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/06 12:20:46 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	remove_quotes(char **lexeme_ptr, int *error)
{
	char	*lexeme;
	char	*result;
	char	quote;
	int		i;
	int		j;

	i = 0;
	j = 0;
	lexeme = *lexeme_ptr;
	result = malloc(ft_strlen(lexeme) + 1);
	if (!check_null(result, error))
		return ;
	//printf("Input to remove_quotes: '%s'\n", *lexeme_ptr);
	//printf("lexeme[i]: '%c'\n", lexeme[i]);
	while (lexeme[i] != '\0')
	{
		if (lexeme[i] == '"' || lexeme[i] == '\'')
		{
			quote = lexeme[i++];
			while (lexeme[i] != '\0' && lexeme[i] != quote)
				result[j++] = lexeme[i++];
			if (lexeme[i] != quote)
			{
				*error = 1;
				return (free(result), print_err(1, 2, "my(s)hell: syntax error\n"));
			}
			i++;
		}
		else
			result[j++] = lexeme[i++];
	}
	result[j] = '\0';
	free(*lexeme_ptr);
	*lexeme_ptr = result;
}

int	process_heredoc_dollar_closed(int file, char *lexeme_no_quotes)
{
	//char	*line;
	char	*buffer;

	while (1)
	{
		buffer = readline("heredoc> ");
		if (buffer == NULL)
			break ;
		//line = get_next_line(fileno(stdin));
		//if (line == NULL)
		//	break ;
		if (g_last_exit_status == 130)
		{
			if (buffer != NULL)
				free(buffer);
			//free(line);
			return (1);
		}
		//buffer = ft_strtrim(line, "\n");
		//free(line);
		if (ft_exact_strcmp(buffer, lexeme_no_quotes) == 0)
		{
			free(buffer);
			return (0);
		}
		write(file, buffer, ft_strlen(buffer));
		write(file, "\n", 1);
		free(buffer);
	}
	return (0);
}

int	process_heredoc_dollar_open(int file, t_exec_vars *vars,
		t_free_data *exec_data, char *lexeme_no_quotes)
{
	//char	*line;
	char	*buffer;
	char	buffer_no_dollar[1024];
	char	*buffer_start;

	while (1)
	{
		buffer = readline("heredoc> ");
		if (buffer == NULL)
			break ;
		//line = get_next_line(fileno(stdin));
		//if (line == NULL)
		//	return (1);
		if (g_last_exit_status == 130)
		{
			if (buffer != NULL)
				free(buffer);
			return (1);
		}
		//buffer = ft_strtrim(line, "\n");
		/*if (buffer == NULL)
		{
			//vars->error = 1;
			return (print_err(1, 2, "my(s)hell: malloc\n"), 1);
		}*/
		//free(line);
		if (ft_exact_strcmp(buffer, lexeme_no_quotes) == 0)
		{
			free(buffer);
			return (0);
		}
		buffer_start = buffer;
		if (handle_dollar_error(&buffer, buffer_no_dollar, vars, exec_data))
			return (free(buffer_start) , 1);
		write(file, buffer_no_dollar, ft_strlen(buffer_no_dollar));
		write(file, "\n", 1);
		free(buffer_start);
	}
	return (0);
}

char	*handle_here_doc(t_p_tree **node, t_exec_vars *vars,
		t_free_data *exec_data)
{
	char	*filename;
	char	*lexeme_no_quotes;
	int		file;

	filename = "/tmp/heredoc.txt";
	lexeme_no_quotes = ft_strdup((*node)->child->data->lexeme);
	if (lexeme_no_quotes == NULL)
	{
		vars->error = 1;
		return (print_err(1, 2, "my(s)hell: malloc\n"), NULL);
	}
	file = open_heredoc_file(filename, vars);
	if (file == -1)
	{
		vars->error = 1;
		return (free(lexeme_no_quotes), NULL);
	}
	if (ft_strpbrk((*node)->child->data->lexeme, "'\"") != NULL)
	{
		remove_quotes(&lexeme_no_quotes, &vars->error);
		if (vars->error)
			return (free(lexeme_no_quotes), close(file), NULL);
		if (process_heredoc_dollar_closed(file, lexeme_no_quotes))
			vars->error = 1;
	}
	else if (process_heredoc_dollar_open(file, vars, exec_data, lexeme_no_quotes))
		//process_heredoc_dollar_open(file, vars, env, lexeme_no_quotes);
		vars->error = 1;// TEST IF FILENAME ACUTALLY PROCESSED
	close(file); //probably no need to free
	free(lexeme_no_quotes);
	return (filename);
}
