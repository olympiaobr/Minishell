/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution10.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:08:57 by jasnguye          #+#    #+#             */
/*   Updated: 2024/05/13 11:09:32 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

int	count_commands(t_data *data)
{
	int		count;
	int		in_command;
	t_token	*current;

	count = 0;
	in_command = 0;
	current = data->token_list;
	while (current != NULL)
	{
		if (current->type == T_COMMAND && !in_command)
		{
			count++;
			in_command = 1;
		}
		else if (current->type == T_PIPE)
		{
			in_command = 0;
		}
		current = current->next;
	}
	data->count_cmd = count;
	return (count);
}

void	close_pipes(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->count_cmd - 1)
	{
		if (data->pipesfd[i])
		{
			close(data->pipesfd[i][0]);
			close(data->pipesfd[i][1]);
			free(data->pipesfd[i]);
		}
		i++;
	}
	free(data->pipesfd);
	data->pipesfd = NULL;
}

int	allocate_and_create_pipes(t_data *data, int num_pipes)
{
	int	i;

	i = 0;
	while (i < num_pipes)
	{
		data->pipesfd[i] = malloc(2 * sizeof(int));
		if (!data->pipesfd[i])
		{
			perror("Failed to allocate memory for individual pipes");
			close_pipes(data);
			return (-1);
		}
		if (pipe(data->pipesfd[i]) == -1)
		{
			perror("Failed to create a pipe");
			close_pipes(data);
			return (-1);
		}
		i++;
	}
	return (0);
}

int	create_pipes(t_data *data)
{
	int	num_pipes;
	int	result;

	num_pipes = data->count_cmd - 1;
	if (num_pipes <= 0)
	{
		printf("No pipes needed.\n");
		return (0);
	}
	printf("Number of pipes to create: %d\n", num_pipes);
	data->pipesfd = malloc(num_pipes * sizeof(int *));
	if (!data->pipesfd)
	{
		perror("Failed to allocate memory for pipe file descriptor arrays");
		return (-1);
	}
	result = allocate_and_create_pipes(data, num_pipes);
	if (result != 0)
	{
		return (result);
	}
	return (0);
}

void	close_all_pipes(t_data *data)
{
	int	index;

	index = 0;
	while (index < data->count_cmd - 1)
	{
		close(data->pipesfd[index][0]);
		close(data->pipesfd[index][1]);
		index++;
	}
}
