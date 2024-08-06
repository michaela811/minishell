/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:37:54 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/06 15:48:00 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"
# include <errno.h>
# include <signal.h>
# include <limits.h>
# include <termios.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <structs.h>
# include <lexer_parser.h>
# include <allocs_and_frees.h>
# include <quotes.h>
# include <env.h>
# include <redirections.h>
# include <utils.h>
# include <exec.h>
# include <builtins.h>
# include <signal.h>
# include <bits/sigaction.h>

#define SYNTAX_ERROR -1
#define SUBTREE_OK 0
#define MEMORY_ERROR 1

extern int	g_last_exit_status;

#endif
