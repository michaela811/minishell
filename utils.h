/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:48 by mmasarov          #+#    #+#             */
/*   Updated: 2024/08/05 18:28:31 by mmasarov         ###   ########.fr       */
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
