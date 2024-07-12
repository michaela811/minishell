/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_here_doc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:21 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/05 16:41:25 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_handle_redirection_here_doc(t_p_tree **node, t_exec_vars *vars, t_here_doc_data *here_docs)
{
	char filename[] = "/tmp/heredocXXXXXX";
	int fd;
	
	fd = mkstemp(filename);
	if (fd == -1)
    {
        vars->error = 1;
        return (print_err(1, 2, "my(s)hell: mkstemp failure\n"), 1);
    }
	pipe_handle_here_doc(node, vars, fd);
	if (vars->error)
	{
		close(fd);
		unlink(filename);
		return (1);
	}
	lseek(fd, 0, SEEK_SET);
	here_docs->fd = fd;
	vars->i++;
	unlink(filename);
	return (0);
}

int	pipe_handle_here_doc(t_p_tree **node, t_exec_vars *vars, int fd)
{
	char    *buffer;
	//int     file;
	char    *line;
	char    *contents = NULL;

	(*node)->data->lexeme = handle_quotes_echo
		((*node)->data->lexeme, &vars->error);
	int original_stdin = dup(STDIN_FILENO);
    if (original_stdin == -1)
    {
       	perror("dup");
      	return 1;
	}
	if (!isatty(STDIN_FILENO))
	{
    	int tty_fd = open("/dev/tty", O_RDONLY);
    	if (tty_fd == -1)
    	{
        	perror("open /dev/tty");
			close(original_stdin);
        	return 1;
    	}
    	if (dup2(tty_fd, STDIN_FILENO) == -1)
    	{
        	perror("dup2 tty_fd to STDIN_FILENO");
			close(tty_fd);
			close(original_stdin);
        	return 1;
    	}
    	close(tty_fd);
	}
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
		if (ft_exact_strcmp(buffer, (*node)->data->lexeme) == 0)
		{
			free(buffer);
			break ;
		}
		if (contents == NULL)
		{
			contents = malloc(ft_strlen(buffer) + 2);
			if (contents == NULL)
				return (free(buffer), 1);
			ft_strcpy(contents, buffer);
		}
		else
		{
			char *temp = realloc(contents, ft_strlen(contents) + ft_strlen(buffer) + 2);
			if (temp == NULL)
				return (free(contents), free(buffer), 1);
			contents = temp;
			ft_strcat(contents, buffer);
		}
		ft_strcat(contents, "\n");
		free(buffer);
	}
	if (contents != NULL)
    {
        write(fd, contents, strlen(contents));
		free(contents);
    }
	return (0);
}

void init_heredocs(t_here_doc_data **here_docs)
{
    if (*here_docs == NULL)
	{
        *here_docs = malloc(sizeof(t_here_doc_data));
        if (*here_docs == NULL)
		{
            perror("malloc failed");
            exit(1);
        }
    }
	else if ((*here_docs)->fd != -1)
        close((*here_docs)->fd);
    (*here_docs)->fd = -1;
}

int	is_there_here_doc(t_p_tree **tree, t_here_doc_data **here_docs)
{
	t_p_tree    *current = *tree;
	t_exec_vars *vars;

	vars = malloc(sizeof(t_exec_vars));
	if (!vars)
	{
		return (print_err(1, 2,
				"my(s)hell: execute_node malloc error\n"), 1);
	}
	init_exec_vars(vars);
	if (current != NULL)
	{
		while (current->child != NULL)
		{ 
			if (current->child->data != NULL && current->child->data->type == HERE_DOC)
			{
				printf("HERE_DOC\n");
				init_heredocs(here_docs);
				if (pipe_handle_redirection_here_doc(&current->child->child, vars, *here_docs) == 0)
					return (free(vars), 0);
			}
			else
				current = current->child;
		}
	}
	free(vars);
	return (1);
}