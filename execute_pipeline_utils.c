/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:36:27 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/02 17:51:45 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*static int	check_return_value(int return_value)
{
	if (return_value != 0)
	{
		g_last_exit_status = return_value;
		return (EXIT_FAILURE);
	}
	g_last_exit_status = 0;
	return (0);
}*/

/*static void	close_fd(int *pipefd)
{
	close(pipefd[0]);
	close(pipefd[1]);
}*/

/*static int	pid2_check(pid_t pid2)
{
	if (pid2 == -1)
		return (print_err(1, 2,
				"my(s)hell: fork in sibling process\n"), 1);
	else
		return (0);
}*/

/*pid_t	handle_sibling_process(int *pipefd, t_free_data *exec_data, t_here_doc_data *here_docs)
{
	pid_t	pid2;
	int		return_value;
	int		here_doc_fd = -1;
	char	*temp_file_path = "/tmp/here_doc_tmp.XXXXXX";

	if (here_docs && here_docs->contents)
	{
        here_doc_fd = mkstemp(temp_file_path);
    	if (here_doc_fd == -1)
		{
            perror("mkstemp");
            exit(EXIT_FAILURE);
        }
        write(here_doc_fd, here_docs->contents, strlen(here_docs->contents));
        close(here_doc_fd);
    }
	pid2 = fork();
	if (pid2_check(pid2))
		return (g_last_exit_status);
	if (pid2 == 0)
	{
        if (here_doc_fd != -1)
		{
            here_doc_fd = open(temp_file_path, O_RDONLY);
            if (here_doc_fd == -1)
			{
                perror("open");
                exit(EXIT_FAILURE);
            }
            // Use the temporary file as STDIN
            if (dup2(here_doc_fd, STDIN_FILENO) == -1)
			{
                perror("dup2");
                exit(EXIT_FAILURE);
            }
            close(here_doc_fd);
			unlink(temp_file_path);
        }
		else
		{
			close(pipefd[1]);
			if (dup2(pipefd[0], STDIN_FILENO) == -1)
			{
				perror("dup2");
				exit(EXIT_FAILURE);
			}
			//close(pipefd[0]);
		}
		//close_fd(pipefd);
		close(pipefd[1]);
        return_value = execute_pipeline(exec_data, here_docs);
        if (check_return_value(return_value))
            exit(EXIT_FAILURE);
        exit(EXIT_SUCCESS);
	}
	else if (pid2 > 0)
	{
        close(pipefd[1]);
        wait(NULL);
        if (here_doc_fd != -1)
            unlink(temp_file_path);
    }
	return (pid2);
}*/
 pid_t handle_sibling_process(int *pipefd, t_free_data *exec_data, t_here_doc_data *here_docs)
{
    pid_t pid2;

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
        return execute_pipeline(exec_data, here_docs);
    }
    else if (pid2 > 0)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        wait(NULL);
    }
    else
    {
        perror("fork");
        return -1;
    }
    return pid2;
}
