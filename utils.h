/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:48 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/06 16:23:18 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	is_directory(const char *path);
int	ft_isspace(char c);
int	directory_check(char *arg_0);
int	is_string_numeric(const char *str);
int	overflow_check(char *result);
void		handle_signal(int signal);
void	setup_signal_handlers(void);
int handle_sigint(int g_last_exit_status, char *buffer);
int	file_error(int file, t_exec_vars *vars);
int	malloc_check(char *no_quotes_lex, int *error);
int	handle_lexeme(char *lexeme, char *quote, char *result, int *error);
int	process_heredoc_dollar_closed(int file, char *no_quotes_lex);
int	break_heredoc(char *buffer);
