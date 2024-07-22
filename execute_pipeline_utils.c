/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:27 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/16 16:42:45 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t handle_sibling_process(int *pipefd, t_free_data *exec_data)
{
    pid_t pid2;
    //int return_value;

    pid2 = fork();
    if (pid2 == -1)
    {
        print_err(errno, 2, "my(s)hell: fork");
        return -1;
    }
    if (pid2 == 0)  // Child process
    {
        if (dup2(pipefd[0], STDIN_FILENO) == -1)
        {
            print_err(errno, 2, "my(s)hell: dup2");
            exit(EXIT_FAILURE);
        }
        close(pipefd[0]);
        close(pipefd[1]);
        exit(execute_pipeline(exec_data));
        //return_value = execute_pipeline(exec_data);
        //g_last_exit_status = return_value;
        /*if (return_value != 0)
        {
            g_last_exit_status = return_value;
            exit(EXIT_FAILURE);
        }
        g_last_exit_status = 0;*/
        //exit(g_last_exit_status);
    }
    else if (pid2 > 0)
    {
        int status;
        if (close(pipefd[0]) == -1)
            print_err(errno, 2, "my(s)hell: close read end of pipe");
        //if (close(pipefd[1]) == -1)
        //    perror("my(s)hell: close write end of pipe");
        waitpid(pid2, &status, 0);
        if (WIFEXITED(status))
            g_last_exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            g_last_exit_status = 128 + WTERMSIG(status);
        else
            g_last_exit_status = -1;
    }
    return pid2;
}
