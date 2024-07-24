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

    printf("Attempting to fork 1...\n");
    pid2 = fork();
    if (pid2 == -1)
        return (print_err(errno, 2, "my(s)hell: fork"), -1);
    if (pid2 == 0)  // Child process
    {
        printf("Child process PID 1: %d\n", getpid());
        if (dup2(pipefd[0], STDIN_FILENO) == -1)
        {
            print_err(errno, 2, "my(s)hell: dup2");
            exit(EXIT_FAILURE);
        }
        close(pipefd[0]);
        close(pipefd[1]);
        exit(execute_pipeline(exec_data));
    }
    else if (pid2 > 0)
    {
        printf("Parent PID: %d, Child PID: %d\n", getpid(), pid2);
        printf("Waiting for child process to terminate...\n");
        int status;
        close(pipefd[0]);
        close(pipefd[1]);
        //if (close(pipefd[1]) == -1)
        //    perror("my(s)hell: close write end of pipe");
        printf("Parent process: %d\n", getpid());
        printf("Child process: %d\n", pid2);
        waitpid(pid2, &status, WNOHANG);
        if (WIFEXITED(status))
            g_last_exit_status = WEXITSTATUS(status);
        else
            g_last_exit_status = -1;
        printf("Child process terminated with status: %d\n", status);
    }
    return (pid2);
}
