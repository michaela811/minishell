/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:48 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/06 15:59:52 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		is_directory(const char *path);
int		ft_isspace(char c);
int		directory_check(char *arg_0);
int		is_string_numeric(const char *str);
int		overflow_check(char *result);
void	handle_signal(int signal);
void	setup_signal_handlers(void);
int 	process_input(t_free_data *exec_data, struct termios *orig_termios);
void	set_raw_mode(struct termios *orig_termios);
void	reset_terminal_mode(struct termios *orig_termios);
