/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:42:32 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/13 10:42:33 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	write_to_heredoc_file(t_data *data, char *delimiter)
{
	char	*new_line;
	int		status;

	new_line = "\n";
	signals_heredoc();
	while (1)
	{
		status = handle_input(data, delimiter, new_line);
		if (status != 0)
		{
			if (status == -1)
				return (-1);
			break ;
		}
		signals_heredoc();
	}
	if (g_heredoc_state == 2)
	{
		printf("warning: heredoc delimited by end-of-file\n");
		g_heredoc_state = 0;
		return (-1);
	}
	return (0);
}

void	check_for_output_file(t_data *data)
{
	t_token	*current;

	current = data->token_list;
	data->output_file = NULL;
	while (current != NULL)
	{
		if (current->type == T_OUT)
		{
			data->output_file = current->next->value;
			data->output_file_present = 1;
		}
		current = current->next;
	}
}

void	check_for_heredoc(t_data *data)
{
	t_token	*current;
	char	*delimiter;

	current = data->token_list;
	data->output_file_present = 0;
	check_for_output_file(data);
	while (current != NULL)
	{
		if (current->type == T_HEREDOC)
		{
			delimiter = current->next->value;
			data->heredoc = 1;
			write_to_heredoc_file(data, delimiter);
		}
		current = current->next;
	}
}
