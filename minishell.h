/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:37:54 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/01 10:37:55 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "libft/libft.h"
#include <errno.h>
#include <signal.h>
#include <limits.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <structs.h>
#include <lexer_parser.h>
#include <allocs_and_frees.h>
#include <quotes.h>
#include <env.h>
#include <redirections.h>
#include <utils.h>
#include <exec.h>
#include <builtins.h>

/*#define MY_MALLOC(size) custom_malloc(size, __FILE__, __LINE__)
#define MY_FREE(ptr) custom_free(ptr, __FILE__, __LINE__)*/

#define SYNTAX_ERROR -1
#define SUBTREE_OK 0
#define MEMORY_ERROR 1

extern int	g_last_exit_status;
void		handle_signal(int signal);

/*Libft*/
/*static void     ft_free(char **array, int j);
static size_t   n_words(const char *str, char c);
static size_t   size_word(const char *s, char c, int i);
char            **ft_split(char const *s, char c);
size_t          ft_strlen(const char *s);
int             ft_strcmp(const char *s1, const char *s2);
char            *ft_strjoin(char const *s1, char const *s2);
void            ft_putstr_fd(char *s, int fd);
void            ft_putendl_fd(char *s, int fd);
static size_t   ft_special_cases(char const *s, unsigned int start, size_t len);
char            *ft_substr(char const *s, unsigned int start, size_t len);
*/

/*
void check_for_memory_leaks();
void custom_free(void *ptr, const char *file, int line);
void *custom_malloc(size_t size, const char *file, int line);*/
