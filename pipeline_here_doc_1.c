/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_here_doc_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 16:15:59 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/18 10:48:43 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pipe_heredoc(t_p_tree **node, t_exec_vars *vars, t_hd_data *here_docs)
{
	char	*filename;
	int		fd;

	filename = "/tmp/heredoc.txt";
	fd = open_heredoc_file(filename, vars);
	pipe_get_heredoc(node, vars, fd);
	if (vars->error)
	{
		close(fd);
		unlink(filename);
		return (1);
	}
	close(fd);
	fd = open(filename, O_RDONLY);
	here_docs->fd = fd;
	vars->i++;
	unlink(filename);
	return (0);
}

void	init_heredocs(t_hd_data **here_docs)
{
	if (*here_docs == NULL)
	{
		*here_docs = malloc(sizeof(t_hd_data));
		if (*here_docs == NULL)
		{
			perror("my(s)hell: malloc failed");
			exit(1);
		}
	}
	else if ((*here_docs)->fd != -1)
		close((*here_docs)->fd);
	(*here_docs)->fd = -1;
}

int	is_there_here_doc(t_p_tree **tree, t_hd_data **here_docs)
{
	t_p_tree	*current;
	t_exec_vars	*vars;

	current = *tree;
	if (current != NULL)
	{
		while (current->child != NULL)
		{
			if (current->child->data != NULL
				&& current->child->data->type == HERE_DOC)
			{
				init_heredocs(here_docs);
				vars = malloc(sizeof(t_exec_vars));
				if (!vars)
					return (print_err(1, 2,
						"my(s)hell: execute_node malloc error\n"), 1);
				init_exec_vars(vars);
				if (pipe_heredoc(&current->child->child, vars, *here_docs) == 0)
					return (free_env_array(vars->args), free(vars), 0);
			}
			else
				current = current->child;
		}
	}
	return (1);
}
