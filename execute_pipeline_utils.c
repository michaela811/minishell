/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:27 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/24 15:02:21 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	handle_sibling_process(int *pipefd, t_free_data *exec_data, t_hd_data *here_docs)
{
	pid_t	pid2;
	int		return_value;

	pid2 = fork();
	if (pid2 == -1)
		return (print_err(errno, 2, "my(s)hell: fork"), -1);
	if (pid2 == 0)
	{
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			print_err(errno, 2, "my(s)hell: dup2");
			exit(EXIT_FAILURE);
		}
		close(pipefd[0]);
		close(pipefd[1]);
		return_value = execute_pipeline(exec_data);
		if (exec_data->token_list_start)
		{
			free_token_list(exec_data->token_list_start);
			exec_data->token_list_start = NULL;
		}
		if (exec_data->tree_start)
		{
			free_parse_tree(exec_data->tree_start);
			exec_data->tree_start = NULL;
		}
		if (exec_data->env)
		{
			free_env(exec_data->env);
			exec_data->env = NULL;
		}
		if (exec_data->environment)
		{
			free_env_array(exec_data->environment);
			exec_data->environment = NULL;
		}
		if (here_docs != NULL)
		{
			close(here_docs->fd);
			free(here_docs);
		}
		exit(return_value);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	return (pid2);
}
