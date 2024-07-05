/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_node_utils_redirections.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:03 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/05 16:22:34 by mmasarov         ###   ########.fr       */
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
	handle_dollar_sign(&start, expanded_lexeme, env, sizeof(expanded_lexeme));
	if (g_last_exit_status)
		return (free(expanded_lexeme));
	if (helper_is_dir(expanded_lexeme, vars))
		return ;
	free(expanded_lexeme);
	vars->fd_out = open((*node)->child->data->lexeme, O_WRONLY | O_CREAT
			| O_APPEND, 0644);
	helper_fd_out_checker(node, vars);
	*node = (*node)->child;
	vars->i++;
}

void	handle_redirection_here_doc(t_p_tree **node, t_exec_vars *vars)
{
	char	*filename;

	filename = handle_here_doc(node, vars);
	if (vars->error)
		return ;
	vars->fd_in = open(filename, O_RDONLY);
	if (vars->fd_in == -1)
	{
		perror("open");
		vars->error = 1;
	}
	*node = (*node)->child;
	vars->i++;
}

char	*handle_here_doc(t_p_tree **node, t_exec_vars *vars)
{
	char	*buffer;
	char	*filename;
	int		file;
	//char	*line;

	filename = "/tmp/heredoc.txt";
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
		buffer = readline("heredoc> ");
		if (buffer == NULL)
			break ;
		/*line = get_next_line(fileno(stdin)); // for tester from this line
		if (line == NULL)
			break ;
		buffer = ft_strtrim(line, "\n");
		free(line); // to this line*/
		if (ft_exact_strcmp(buffer, (*node)->child->data->lexeme) == 0)
		{
			free(buffer);
			break ;
		}
		write(file, buffer, ft_strlen(buffer));
		write(file, "\n", 1);
		free(buffer);
	}
	close(file);
	return (filename);
}
