/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:27 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/05 11:33:06 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t handle_sibling_process(int *pipefd, t_free_data *exec_data)//, t_here_doc_data *here_docs)
{
    pid_t pid2;
    int return_value;

    pid2 = fork();
    if (pid2 == -1)
    {
        perror("fork");
        return -1;
    }
    if (pid2 == 0)  // Child process
    {
        if (dup2(pipefd[0], STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }
        close(pipefd[0]);
        close(pipefd[1]);
        return_value = execute_pipeline(exec_data);//, here_docs);
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
        int status;
        if (close(pipefd[0]) == -1)
            perror("my(s)hell: close read end of pipe");
        /*if (close(pipefd[1]) == -1)
            perror("my(s)hell: close write end of pipe");*/
        if (waitpid(pid2, &status, 0) == -1)
        {
            perror("my(s)hell: waitpid");
            return -1;
        }
        if (WIFEXITED(status))
            g_last_exit_status = WEXITSTATUS(status);
        else
            g_last_exit_status = -1;
    }
    return pid2;
}
