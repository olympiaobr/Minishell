/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 15:23:23 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/11 20:05:14 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"
volatile sig_atomic_t heredoc_interrupted_flag = 0;

void catch_sigint(int sig)
{
   (void)sig;
	if(sig == SIGINT) // does not work
	{
		if(heredoc_mode == 1)
		{
			heredoc_interrupted_flag = 1;
		}
		
	}
	
	ft_putchar_fd('\n', STDOUT_FILENO);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void handle_sigquit(int signum)
{
    if (signum == SIGQUIT)
    {
        printf("Quit (core dumped)\n");
        rl_on_new_line();
    }
}

void setup_interactive_signals(void)
{
    struct sigaction action_int, action_quit;

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

void setup_noninteractive_signals(void)
{
    struct sigaction action;

    ft_memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = handle_sigquit;
    sigemptyset(&action.sa_mask);
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGQUIT, &action, NULL);
}

// Signal setup caller function based on the mode
void signal_setup(int mode)
{
    if (mode == 1)
    {
        setup_interactive_signals();
    }
    else
    {
        setup_noninteractive_signals();
    }
}



void heredoc_signals(void) 
{
    struct sigaction action_int, action_quit;

    //ctrl + c
    ft_memset(&action_int, 0, sizeof(struct sigaction));
    action_int.sa_handler = catch_sigint;
    sigemptyset(&action_int.sa_mask);
    action_int.sa_flags = 0;
    sigaction(SIGINT, &action_int, NULL);
	
    // (Ctrl+\)
    ft_memset(&action_quit, 0, sizeof(struct sigaction));
    action_quit.sa_handler = SIG_IGN;
    sigemptyset(&action_quit.sa_mask);
    action_quit.sa_flags = 0;
    sigaction(SIGQUIT, &action_quit, NULL);

    // (Ctrl+Z) //ctrl + d
    struct sigaction action_tstp;
    ft_memset(&action_tstp, 0, sizeof(struct sigaction));
    action_tstp.sa_handler = SIG_IGN;
    sigemptyset(&action_tstp.sa_mask);
    action_tstp.sa_flags = 0;
    sigaction(SIGTSTP, &action_tstp, NULL);
}
