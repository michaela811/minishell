/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_parse_tree.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:15 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/18 17:22:12 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int open_heredoc_file(char *filename, t_exec_vars *vars)
{
    int file;

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
				return(free(result), print_err(1, 2, "my(s)hell: syntax error\n"));
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

void process_heredoc_dollar_closed(int file, char *lexeme_no_quotes)
{
    char *line;
    char *buffer;

    while (1)
	{
        line = get_next_line(fileno(stdin));
        if (line == NULL)
			break;
        buffer = ft_strtrim(line, "\n");
        free(line);
        if (ft_exact_strcmp(buffer, lexeme_no_quotes) == 0)
		{
            free(buffer);
            break;
        }
        write(file, buffer, ft_strlen(buffer));
        write(file, "\n", 1);
        free(buffer);
    }
}

void process_heredoc_dollar_open(int file, t_exec_vars *vars, t_env **env,
		char *lexeme_no_quotes)
{
    char *line;
    char *buffer;
    char buffer_no_dollar[1024];
	char *buffer_start;

    while (1)
	{
        line = get_next_line(fileno(stdin));
        if (line == NULL)
			break;
        buffer = ft_strtrim(line, "\n");
        free(line);
        if (ft_exact_strcmp(buffer, lexeme_no_quotes) == 0)
		{
            free(buffer);
            break;
        }
        buffer_start = buffer;
        if (handle_dollar_error(&buffer, buffer_no_dollar, vars, env))
            return (free(buffer_start));
        write(file, buffer_no_dollar, ft_strlen(buffer_no_dollar));
        write(file, "\n", 1);
        free(buffer_start);
    }
}

char *handle_here_doc(t_p_tree **node, t_exec_vars *vars, t_env **env)
{
    char *filename = "/tmp/heredoc.txt";
    char *lexeme_no_quotes = ft_strdup((*node)->child->data->lexeme);
    if (lexeme_no_quotes == NULL)
		return (NULL);
    int file = open_heredoc_file(filename, vars);
    if (file == -1) {
        free(lexeme_no_quotes);
        return NULL;
    }
    if (ft_strpbrk((*node)->child->data->lexeme, "'\"") != NULL)
	{
        remove_quotes(&lexeme_no_quotes, &vars->error);
        if (vars->error)
			return (free(lexeme_no_quotes), close(file), NULL);
		process_heredoc_dollar_closed(file, lexeme_no_quotes);
	}
	else
    	process_heredoc_dollar_open(file, vars, env, lexeme_no_quotes);
    close(file);
    free(lexeme_no_quotes);
    return filename;
}

/* char	*handle_here_doc(t_p_tree **node, t_exec_vars *vars, t_env **env)
{
	char	*buffer;
	char	*filename;
	int		file;
	char	*line;
	char *lexeme_no_quotes;

	filename = "/tmp/heredoc.txt";
	lexeme_no_quotes = ft_strdup((*node)->child->data->lexeme);
	if (lexeme_no_quotes == NULL)
		return (NULL);

	file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (file == -1)
	{
		vars->error = 1;
		return (print_err(1, 2, "my(s)hell: open error\n"), free(lexeme_no_quotes), NULL);//don't know whether it's better over perror
		//return ((vars->error = 1), perror("open"), NULL);
	}
	if (ft_strpbrk((*node)->child->data->lexeme, "'\"") == NULL)
	{
		char	buffer_no_dollar[1024];
		while (1)
		{
		//buffer = readline("heredoc> ");
		//if (buffer == NULL)
		//	break ;
		line = get_next_line(fileno(stdin)); // for tester from this line
		if (line == NULL)
			break ;
		buffer = ft_strtrim(line, "\n");
		free(line); // to this line
		if (ft_exact_strcmp(buffer, (*node)->child->data->lexeme) == 0)
		{
			free(buffer);
			break ;
		}
		char *buffer_start = buffer;
		if (handle_dollar_error(&buffer, buffer_no_dollar, vars, env))
			return (free(buffer), close(file), NULL);
		write(file, buffer_no_dollar, ft_strlen(buffer_no_dollar));
		write(file, "\n", 1);
		free(buffer_start);
		//free(buffer);
		}
	}
	else
	{
		remove_quotes(&lexeme_no_quotes, &vars->error);
		if (vars->error)
			return (free(lexeme_no_quotes), close(file), NULL);
		while (1)
		{
		//buffer = readline("heredoc> ");
		//if (buffer == NULL)
		//	break ;
		line = get_next_line(fileno(stdin)); // for tester from this line
		if (line == NULL)
			break ;
		buffer = ft_strtrim(line, "\n");
		free(line); // to this line
		if (ft_exact_strcmp(buffer, (*node)->child->data->lexeme) == 0)
		{
			free(buffer);
			break ;
		}
		write(file, buffer, ft_strlen(buffer));
		write(file, "\n", 1);
		free(buffer);
		}
	}
	close(file);
	return (filename);
} */

/* void process_heredoc_content(int file, t_exec_vars *vars, t_env **env,
		char *lexeme_no_quotes)
{
    char *line;
    char *buffer;
    char buffer_no_dollar[1024];
	char *buffer_start;

    while (1)
	{
        line = get_next_line(fileno(stdin));
        if (line == NULL)
			break;
        buffer = ft_strtrim(line, "\n");
        free(line);
        if (ft_exact_strcmp(buffer, lexeme_no_quotes) == 0)
		{
            free(buffer);
            break;
        }
        buffer_start = buffer;
        if (ft_strpbrk(lexeme_no_quotes, "'\"") == NULL)
		{
            if (handle_dollar_error(&buffer, buffer_no_dollar, vars, env))
                return (free(buffer_start));
            write(file, buffer_no_dollar, ft_strlen(buffer_no_dollar));
        }
		else
            write(file, buffer, ft_strlen(buffer));
        write(file, "\n", 1);
        free(buffer_start);
    }
} */
