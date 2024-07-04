/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:27 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/04 13:39:49 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	handle_sibling_process(int *pipefd, t_free_data *exec_data,
		t_here_doc_data *here_docs)
{
    pid_t	pid2;
    int		return_value;

    pid2 = fork();
	if (pid2 == -1)
		return (print_err(1, 2, "my(s)hell: fork in sibling process\n"), 1);
    if (pid2 == 0)
    {
        if (dup2(pipefd[0], STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(pipefd[0]);
		close(pipefd[1]);
        return_value = execute_pipeline(exec_data, here_docs);
        if (return_value != 0)
        {
            g_last_exit_status = return_value;
            exit(EXIT_FAILURE);
        }
        g_last_exit_status = 0;
        exit(EXIT_SUCCESS);
    }
    else if (pid2 > 0)
    {
        close(pipefd[0]);
		close(pipefd[1]);//New trial
        wait(NULL);
    }
    else
    {
        perror("fork");
        return -1;
    }
    return pid2;
}