/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_here_doc_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:15:59 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/06 20:24:19 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* void handle_heredoc_quotes(int fd, t_p_tree **node, t_exec_vars *vars,
		t_free_data *exec_data) */
int	handle_heredoc_quotes(int fd, t_p_tree **node,
		t_free_data *exec_data)
{
    char	*no_quotes_lex;

    no_quotes_lex = ft_strdup((*node)->data->lexeme);
    if (no_quotes_lex == NULL)
		return (print_err(1, 2, "my(s)hell: malloc\n"), 1);
    if (ft_strpbrk((*node)->data->lexeme, "'\"") != NULL)
    {
        if (remove_quotes(&no_quotes_lex) == 0)
        {
			if (pipe_heredoc_dollar_closed(no_quotes_lex, fd))
				return(free(no_quotes_lex), g_last_exit_status);
		}
    }
    else if (pipe_heredoc_dollar_open(no_quotes_lex, fd, exec_data))
        return(free(no_quotes_lex), g_last_exit_status);
    return (free(no_quotes_lex), 0);
}
int	pipe_heredoc(t_p_tree **node, int *here_docs, t_free_data *exec_data)
/* int	pipe_heredoc(t_p_tree **node, t_exec_vars *vars, int *here_docs,
		t_free_data *exec_data) */
{
	char	*filename;
	int		fd;

	filename = "/tmp/heredoc.txt";
	fd = open_heredoc_file(filename);
	if (fd == -1)
		return (g_last_exit_status);
	if (handle_heredoc_quotes(fd, node, exec_data))
	{
		close(fd);
		unlink(filename);
		return (1);
	}
	close(fd);
	fd = open(filename, O_RDONLY);
	*here_docs = fd;
	//vars->i++;
	unlink(filename);
	return (0);
}
//int handle_pipe_heredoc(t_exec_vars **vars, int *here_docs,
		//t_p_tree *current, t_free_data *exec_data)
int handle_pipe_heredoc(int *here_docs, t_p_tree *current,
		t_free_data *exec_data)
{
    if (*here_docs != -1)
    {
        //free_array((*vars)->args);
        //free(*vars);
        //*vars = NULL;
        close(*here_docs);
        *here_docs = -1;
    }
    //*vars = malloc(sizeof(t_exec_vars));
    //if (!*vars)
        //return (print_err(1, 2, "my(s)hell: execute_node malloc error\n"), 1);
    //init_exec_vars(*vars);
    pipe_heredoc(&current->child->child, here_docs, exec_data);
    if (g_last_exit_status == 130)
    {
        //free_array((*vars)->args);
        //free(*vars);
        return (1);
    }
    return (0);
}

int	is_there_here_doc(t_p_tree **tree, int *here_docs, t_free_data *exec_data)
{
	t_p_tree	*current;
	//t_exec_vars	*vars = NULL;

	current = *tree;
	if (current != NULL)
	{
		while (current->child != NULL)
		{
			if (current->child->data != NULL
				&& current->child->data->type == HERE_DOC)
			{
				if (handle_pipe_heredoc(here_docs, current, exec_data))
					return (g_last_exit_status);
			}
			current = current->child;
		}
	}
	/* if (*here_docs != -1)
	{
		free_array(vars->args);
		free(vars);
	} */
	return (0);
}
