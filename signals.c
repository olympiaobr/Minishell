/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 15:23:23 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/12 18:19:11 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

void	catch_sigint(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigquit(int signum)
{
	if (signum == SIGQUIT)
	{
		printf("Quit (core dumped)\n");
		rl_on_new_line();
	}
}

void	setup_interactive_signals(void)
{
	struct sigaction	action_int;
	struct sigaction	action_quit;

	ft_memset(&action_int, 0, sizeof(struct sigaction));
	action_int.sa_handler = catch_sigint;
	sigemptyset(&action_int.sa_mask);
	action_int.sa_flags = 0;
	sigaction(SIGINT, &action_int, NULL);
	ft_memset(&action_quit, 0, sizeof(struct sigaction));
	action_quit.sa_handler = SIG_IGN;
	sigemptyset(&action_quit.sa_mask);
	action_quit.sa_flags = 0;
	sigaction(SIGQUIT, &action_quit, NULL);
}

void	setup_noninteractive_signals(void)
{
	struct sigaction	action;

	ft_memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = handle_sigquit;
	sigemptyset(&action.sa_mask);
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGQUIT, &action, NULL);
}
