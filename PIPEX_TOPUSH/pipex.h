/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 15:51:03 by mmasarov          #+#    #+#             */
/*   Updated: 2024/01/30 17:20:03 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <errno.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>  
# include <stdlib.h>

void	execute(char *cmd, char **env);
void	child(char **argv, int *p_fd, char **env);
void	parent(char **argv, int *p_fd, char **env);
void	exit_function(int i);
int		open_file(char *file, int in_out);
char	*get_path(char *cmd, char **env);
char	*get_env(char **env, char *var);
void	free_array(char **tab);
void	error_message(char *str);
void	execve_error(char **s_cmd);
char	*get_exec(char **path, int i, char **s_cmd);

#endif
