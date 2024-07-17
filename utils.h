/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 10:38:48 by mmasarov          #+#    #+#             */
/*   Updated: 2024/07/17 15:15:56 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	is_directory(const char *path);
int	ft_isspace(char c);
int	directory_check(char *arg_0);
int	is_string_numeric(const char *str);
int	overflow_check(char *result);

//to delete later(used only for debugging)
void	print_args(char **args);