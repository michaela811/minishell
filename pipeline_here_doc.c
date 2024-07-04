/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_here_doc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:21 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/04 16:33:27 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *pipe_handle_redirection_here_doc(t_p_tree **node, t_exec_vars *vars)
{
	char    *contents;
	char    *filename;

	filename = "/tmp/heredoc.txt";
	contents = pipe_handle_here_doc(node, vars, filename);
	if (vars->error)
		return (NULL);
	vars->fd_in = open(filename, O_RDONLY);
	if (vars->fd_in == -1)
	{
		perror("open");
		vars->error = 1;
	}
	*node = (*node)->child;
	vars->i++;
	return (contents);
}

char    *pipe_handle_here_doc(t_p_tree **node, t_exec_vars *vars, char *filename)
{
	char    *buffer;
	int     file;
	char    *line;
	char    *contents = NULL;

	(*node)->child->data->lexeme = handle_quotes_echo
		((*node)->child->data->lexeme, &vars->error);
	file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	if (file == -1)
	{
		vars->error = 1;
		return ((vars->error = 1), perror("open"), NULL);
	}
	while (1)
	{
		/*buffer = readline("heredoc> ");
		if (buffer == NULL)
			break ;*/
		line = get_next_line(fileno(stdin)); // for tester from this line
		if (line == NULL)
			break ;
		buffer = ft_strtrim(line, "\n");
		free(line); // to this line
		if (ft_strcmp(buffer, (*node)->child->data->lexeme) == 0)
		{
			free(buffer);
			break ;
		}
		if (contents == NULL)
		{
			contents = malloc(ft_strlen(buffer) + 2);
			 if (contents == NULL)
			{
				free(buffer);
				return NULL;
			}
			strcpy(contents, buffer);
		}
		else
		{
			char *temp = realloc(contents, ft_strlen(contents) + ft_strlen(buffer) + 2);
			if (temp == NULL)
			{

				free(contents);
				free(buffer);
				return NULL;
			}
			contents = temp;
			strcat(contents, buffer);
		}
		ft_strcat(contents, "\n");
		free(buffer);
	}
	close(file);
	return (contents);
}

char    *pipe_handle_redirection(t_p_tree **node, t_exec_vars *vars)
{
	if ((*node)->data->type == HERE_DOC)
		return (pipe_handle_redirection_here_doc(node, vars));
	return (NULL);
}

char    *pipe_handle_node_data(t_p_tree **node, t_exec_vars *vars)
{
	if ((*node)->data->type == RED_FROM || (*node)->data->type == RED_TO
		|| (*node)->data->type == APPEND || (*node)->data->type == HERE_DOC)
		return (pipe_handle_redirection(node, vars));
	return (NULL);
}

void    is_there_here_doc(t_p_tree **tree, t_here_doc_data **here_docs)
{
	t_p_tree    *current = *tree;
	t_exec_vars *vars;
	char        *contents = NULL;

	vars = malloc(sizeof(t_exec_vars));
	if (!vars)
	{
		return (print_err(1, 2,
				"my(s)hell: execute_node malloc error\n"));
	}
	init_exec_vars(vars);
	while (current != NULL)
	{
		while (current->child != NULL && current->child->child != NULL && current->child->child->data != NULL)
		{
			if (current->child->child->data->type == HERE_DOC)
			{
				contents = pipe_handle_node_data(&current->child->child, vars);
				if (contents != NULL)
				{
					t_here_doc_data *new_here_doc = malloc(sizeof(t_here_doc_data));
			   		new_here_doc->contents = contents;
					new_here_doc->next = *here_docs;
					*here_docs = new_here_doc;
					current->child->child->data = NULL;
				}
			}
			current->child = current->child->child;
		}
		if (current->sibling != NULL && current->sibling->data != NULL)
			is_there_here_doc(&current->child, here_docs);
		current = current->sibling;
	}
	free(vars);
}
