/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 15:39:27 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/02 16:08:27 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include "Libft/libft.h"

void count_commands(t_data *data)
{
    int count = 0;
    int in_command = 0;
    t_token *current = data->token_list;

    while (current != NULL)
	{
        if (current->type == T_COMMAND && !in_command)
		{
            count++;
            in_command = 1;
        }
		else if (current->type == T_PIPE)
		{
            in_command = 0;    // reset command context after a pipe
        }
        current = current->next;
    }
    data->count_cmd = count; // Update command count
}

int setup_filedescriptors_for_redirection(t_data *data)
{
	int flags;
	if(data->input_file)
	{
		data->std_input_fd = open(data->input_file, O_RDONLY);
		if(data->std_input_fd == -1)
		{
			perror("Failed to open input file");
			data->std_input_fd = STDIN_FILENO; // = 0
			return(-1);
		}
		
	}
	else if(data->output_file)
	{
		if(data->append)
		{
			flags =  O_WRONLY | O_CREAT | O_APPEND;
		}
		else
		{
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		}
		data->std_output_fd = open(data->output_file, flags, 0644);
		if(data->std_output_fd == -1)
		{
			perror("Failed to open output file");
			data->std_output_fd = STDOUT_FILENO; // = 1
			return(-1);
		}
	}
	return(0);
}

int create_pipes(t_data *data)
{
	int num_pipes = data->count_cmd - 1;
	int i = 0;
	data->pipesfd = malloc(2 * sizeof(int[2]));
	if(!data->pipesfd)
	{
		perror("Failed to allocate memory for pipes");
		return (-1);
	}
	while(i < num_pipes)
	{
		data->pipesfd[i] = malloc(2 * sizeof(int));// is this correct?
		if(!data->pipesfd[i])
		{
			perror("Failed to allocate memory for pipe");
			int j = 0;
			while(j < i)
			{
				free(data->pipesfd[j]);
				j++;
			}
			free(data->pipesfd);
			data->pipesfd = NULL;
			return (-1);
		}
		if(pipe(data->pipesfd[i]) == -1)
		{
			perror("Failed to create pipe");
			int j = 0;
			while(j <= i)
			{
				close(data->pipesfd[j][0]);
				close(data->pipesfd[j][1]);
				close(data->pipesfd[j]);
				j++;
			}
			free(data->pipesfd);
			data->pipesfd = NULL;
			return (-1);
		}
		i++;
	}
	return(0);
}

void execute_pipeline(t_data *data, t_command *cmd)
{
	
}