/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:48 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/08 11:29:34 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		is_directory(const char *path);
int		ft_isspace(char c);
int		directory_check(char *arg_0);
int		is_string_numeric(const char *str);
int		overflow_check(char *result);
void	handle_signal(int signal);
void	setup_signal_handlers(void);
int		malloc_check(char *no_quotes_lex, int *error);
int		handle_lexeme(char *lexeme, char *quote, char *result);
int		process_heredoc_dollar_closed(int file, char *no_quotes_lex);
int		break_heredoc(char *buffer);
int		process_input(t_free_data *exec_data);
void	set_raw_mode(struct termios *orig_termios);
void	reset_terminal_mode(struct termios *orig_termios);
int		fork_check(pid_t pid, int *pipefd);
void	helper_free_close_open(t_p_tree **node, char *exp_lexeme,
			t_exec_vars *vars);
void	custom_sigquit_handler(int sig);
void	helper_sigquit_handler(struct sigaction *sa, struct sigaction *old_sa);
char	*find_first_non_alnum(char *str);