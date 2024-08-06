/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:48 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/06 20:20:35 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		is_directory(const char *path);
int		ft_isspace(char c);
int		directory_check(char *arg_0);
int		is_string_numeric(const char *str);
int		overflow_check(char *result);
void	handle_signal(int signal);
void	setup_signal_handlers(void);
int		handle_sigint(int g_last_exit_status, char *buffer);
int		file_error(int file, t_exec_vars *vars);
int		malloc_check(char *no_quotes_lex, int *error);
int		handle_lexeme(char *lexeme, char *quote, char *result);
int		process_heredoc_dollar_closed(int file, char *no_quotes_lex);
int		break_heredoc(char *buffer);
int 	process_input(t_free_data *exec_data, struct termios *orig_termios);
void	set_raw_mode(struct termios *orig_termios);
void	reset_terminal_mode(struct termios *orig_termios);
