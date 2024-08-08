/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpadenko <dpadenko@student.42vienna.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 14:19:21 by dpadenko          #+#    #+#             */
/*   Updated: 2024/08/06 14:22:04 by dpadenko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signal(int sig)
{
	int	saved_errno;

	if (sig == SIGINT)
	{
		g_last_exit_status = 130;
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
	{
	}
	else if (sig == SIGCHLD || sig == SIGPIPE)
	{
		saved_errno = errno;
		while (waitpid((pid_t)(-1), 0, WNOHANG) > 0)
			continue ;
		errno = saved_errno;
	}
}

void	setup_signal_handlers(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
	if (sigaction(SIGCHLD, &sa, NULL) == -1)
	{
		print_err(errno, 2, "my(s)hell: sigaction sigchild");
		exit(EXIT_FAILURE);
	}
	if (sigaction(SIGPIPE, &sa, NULL) == -1)
	{
		print_err(errno, 2, "my(s)hell: sigaction sigpipe");
		exit(EXIT_FAILURE);
	}
	signal(SIGINT, handle_signal);
	(signal(SIGQUIT, SIG_IGN));
}

void	custom_sigquit_handler(int sig)
{
	if (sig == SIGQUIT)
	{
		g_last_exit_status = 131;
		return ;
	}
}

void	helper_sigquit_handler(struct sigaction *sa, struct sigaction *old_sa)
{
	sa->sa_handler = custom_sigquit_handler;
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = 0;
	sigaction(SIGQUIT, sa, old_sa);
}
