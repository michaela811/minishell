/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 13:58:33 by mmasarov          #+#    #+#             */
/*   Updated: 2023/10/04 14:02:30 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_FD_H
# define FT_PRINTF_FD_H

# include <stdio.h>
# include <stdarg.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>

int		ft_print_char_fd(unsigned int fd, char c);
int		ft_hex_len(unsigned int num);
void	ft_put_hex_fd(unsigned int fd, unsigned int num, char placeholder);
int		ft_print_hex_fd(unsigned int fd, unsigned int num, char placeholder);
int		ft_int_len(int num);
void	ft_put_int_fd(unsigned int fd, int num);
int		ft_print_int_fd(unsigned int fd, int nbr);
int		ft_print_percent_fd(unsigned int fd);
int		ft_ptr_len(unsigned long long num);
void	ft_put_ptr_fd(unsigned int fd, unsigned long long ptr);
int		ft_print_ptr_fd(unsigned int fd, unsigned long long ptr);
int		ft_print_str_fd(unsigned int fd, const char *str);
int		ft_unsign_int_len(unsigned int num);
char	*ft_put_unsign_int(unsigned int num);
int		ft_print_unsign_int_fd(unsigned int fd, unsigned int num);
int		ft_format_fd(unsigned int fd, char placeholder, va_list args);
int		ft_printf_fd(unsigned int fd, const char *print, ...);

#endif
