/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 12:42:54 by jasnguye          #+#    #+#             */
/*   Updated: 2024/05/12 16:20:40 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

volatile sig_atomic_t	g_heredoc_state = 0;

void	append_input(t_data *data, char *input, char *new_line,
		size_t new_input_length)
{
	char	*temp;

	new_input_length = ft_strlen(data->heredoc_input) + ft_strlen(input)
		+ ft_strlen(new_line) + 1;
	temp = malloc(new_input_length);
	if (temp == NULL)
	{
		printf("Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
	ft_strcpy(temp, data->heredoc_input);
	ft_strcat(temp, new_line);
	ft_strcat(temp, input);
	free(data->heredoc_input);
	data->heredoc_input = temp;
}

void	heredoc_sig(int sig)
{
	if (sig == SIGINT)
	{
		g_heredoc_state = 2;
		write(STDOUT_FILENO, "\n^C\n", 4);
		rl_replace_line("", 0);
		rl_redisplay();
		rl_done = 1;
	}
	else if (sig == SIGQUIT)
	{
		write(STDOUT_FILENO, "\nQuit (core dumped)\n", 20);
		exit(1);
	}
}

void	signals_heredoc(void)
{
	struct sigaction	sig;
	struct sigaction	quit_sig;

	ft_memset(&sig, 0, sizeof(struct sigaction));
	sig.sa_handler = heredoc_sig;
	sigemptyset(&sig.sa_mask);
	sig.sa_flags = 0;
	ft_memset(&quit_sig, 0, sizeof(struct sigaction));
	quit_sig.sa_handler = SIG_IGN;
	sigemptyset(&quit_sig.sa_mask);
	quit_sig.sa_flags = 0;
	sigaction(SIGINT, &sig, NULL);
	sigaction(SIGQUIT, &quit_sig, NULL);
}

int	append_to_heredoc(t_data *data, char *input, char *new_line)
{
	char	*temp;
	int		len;

	if (data->heredoc_input == NULL)
	{
		data->heredoc_input = ft_strdup(input);
	}
	else
	{
		len = ft_strlen(data->heredoc_input) + ft_strlen(input)
			+ ft_strlen(new_line) + 1;
		temp = malloc(len);
		if (temp == NULL)
		{
			return (-1);
		}
		ft_strcpy(temp, data->heredoc_input);
		ft_strcat(temp, new_line);
		ft_strcat(temp, input);
		free(data->heredoc_input);
		data->heredoc_input = temp;
	}
	return (0);
}

int	handle_input(t_data *data, char *delimiter, char *new_line)
{
	char	*input;
	int		result;

	input = readline("> ");
	if (!input || g_heredoc_state == 2)
	{
		free(input);
		return (1);
	}
	if (ft_strcmp(input, delimiter) == 0)
	{
		free(input);
		return (1);
	}
	result = append_to_heredoc(data, input, new_line);
	if (result == -1)
	{
		free(input);
		return (-1);
	}
	free(input);
	return (0);
}
