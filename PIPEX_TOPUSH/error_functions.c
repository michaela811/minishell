/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 09:37:10 by mmasarov          #+#    #+#             */
/*   Updated: 2024/01/30 17:19:20 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_message(char *str)
{
	perror(str);
	exit (1);
}

void	free_array(char **array)
{
	size_t	i;

	i = -1;
	while (array[++i])
		free(array[i]);
	free(array);
}

void	exit_function(int i)
{
	if (i == 1)
		ft_putstr_fd("Please enter correct input:\n./pipex"
			" infile \"cmd\" \"cmd\" outfile\n", 2);
	else if (i == 2)
		ft_putstr_fd("The infile does not exists.\n", 2);
	exit(0);
}

void	execve_error(char **s_cmd)
{
	perror("Execve failed");
	ft_putendl_fd("This command was not found: ", 2);
	ft_putendl_fd(s_cmd[0], 2);
	free_array(s_cmd);
	exit(0);
}
