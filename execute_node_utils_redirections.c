/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_utils_redirections.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:03 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/18 17:19:09 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	helper_is_dir(char	*expanded_lexeme, t_exec_vars *vars)
{
	int	is_dir;

	is_dir = is_directory(expanded_lexeme);
	if (is_dir == 1)
	{
		vars->error = 1;
		print_err(1, 2, "my(s)hell: %s: Is a directory\n",
			expanded_lexeme);
		free(expanded_lexeme);
		return (1);
	}
	return (0);
}

static void	helper_fd_out_checker(t_p_tree **node, t_exec_vars *vars)
{
	if (vars->fd_out == -1)
	{
		vars->error = 1;
		if (errno == EACCES)
			print_err(1, 2, "my(s)hell: %s: Permission denied\n",
				(*node)->child->data->lexeme);
		else
			print_err(1, 2,
				"my(s)hell: %s: No such file or directory\n",
				(*node)->child->data->lexeme);
	}
}

void	handle_redirection_append(t_p_tree **node, t_exec_vars *vars,
			t_env **env)
{
	char	*start;
	char	*expanded_lexeme;

	quotes_glob_redirect(node, vars, env);
	if (g_last_exit_status)
		return ;
	expanded_lexeme = malloc(4096);
	if (!expanded_lexeme)
	{
		perror("malloc");
		vars->error = 1;
		return ;
	}
	ft_memset(expanded_lexeme, '\0', sizeof(expanded_lexeme));
	start = (*node)->child->data->lexeme;
	if (handle_dollar_sign(&start, expanded_lexeme, env, sizeof(expanded_lexeme)))
	//if (g_last_exit_status)
		return (free(expanded_lexeme));
	if (helper_is_dir(expanded_lexeme, vars))
		return ;
	free(expanded_lexeme);
	vars->fd_out = open((*node)->child->data->lexeme, O_WRONLY | O_CREAT
			| O_APPEND, 0644);
	helper_fd_out_checker(node, vars);
	*node = (*node)->child;
	//vars->i++;
}

void	handle_redirection_here_doc(t_p_tree **node, t_exec_vars *vars ,
		t_hd_data *here_docs, t_env **env)
{
	char	*filename;

	if (here_docs && here_docs->fd != -1)
		vars->fd_in = here_docs->fd;
	else
	{
		filename = handle_here_doc(node, vars, env);
		if (vars->error)
			return ;
		vars->fd_in = open(filename, O_RDONLY);
		if (vars->fd_in == -1)
		{
			perror("open");
			vars->error = 1;
		}
	}
	*node = (*node)->child;
	//vars->i++;? we shouldn't increment i here?
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

void process_heredoc_content(int file, t_exec_vars *vars, t_env **env,
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
}

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
