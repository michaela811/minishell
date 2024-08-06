/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmasarov <mmasarov@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 14:18:04 by dpadenko          #+#    #+#             */
/*   Updated: 2024/08/06 18:37:53 by mmasarov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				g_last_exit_status = 0;

void	reset_terminal_mode(struct termios *orig_termios)
{
	tcsetattr(STDIN_FILENO, TCSANOW, orig_termios);
}

void	set_raw_mode(struct termios *orig_termios)
{
	struct termios	raw;

	ft_memset(orig_termios, 0, sizeof(struct termios));
	tcgetattr(STDIN_FILENO, orig_termios);
	raw = *orig_termios;
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
	raw.c_cflag |= (CS8);
	raw.c_oflag &= ~(OPOST);
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 0;
	raw.c_lflag |= ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}
