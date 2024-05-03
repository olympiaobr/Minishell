/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 15:39:27 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/03 12:21:01 by jasnguye         ###   ########.fr       */
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
				free(data->pipesfd[j]);
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

void wait_and_close_pipes(t_data *data, int num_processes)
{
    int status;
    pid_t pid;
    int exit_status = 0;

    while (num_processes > 0)
	{
        pid = wait(&status);
        if (pid == -1) {
            perror("wait");
            continue;
        }
        if (WIFEXITED(status))
		{
            int current_status = WEXITSTATUS(status);
            printf("Process %d exited with status: %d\n", pid, current_status);
            exit_status = (current_status > exit_status) ? current_status : exit_status;
        }
        num_processes--;
    }
    data->exit_status = exit_status;
    int i = 0;
    while (i < data->count_cmd - 1) {
        close(data->pipesfd[i][0]);
        close(data->pipesfd[i][1]);
        i++;
    }
}


void execute_pipeline(t_data *data, t_command *cmd)
{
    t_command *current_cmd = cmd;
    int j = 0;

    while (current_cmd != NULL)
	{
        pid_t pid = fork();
        if (pid == -1)
		{
            perror("fork");
            exit(EXIT_FAILURE);
        }
		else if (pid == 0)
		{ // Child process
            if (j != 0) {
                if (dup2(data->pipesfd[j - 1][0], STDIN_FILENO) == -1)
				{
                    perror("dup2 stdin");
                    exit(EXIT_FAILURE);
                }
            }
            if (j != data->count_cmd - 1)
			{
                if (dup2(data->pipesfd[j][1], STDOUT_FILENO) == -1)
				{
                    perror("dup2 stdout");
                    exit(EXIT_FAILURE);
                }
            }
            int i = 0;
            while (i < data->count_cmd - 1)
			{
                close(data->pipesfd[i][0]);
                close(data->pipesfd[i][1]);
                i++;
            }
            execvp(current_cmd->command, (char *[]){current_cmd->command, NULL});
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        current_cmd = current_cmd->next;
        j++;
    }
    int i = 0;
    while (i < data->count_cmd - 1)
	{
        close(data->pipesfd[i][0]);
        close(data->pipesfd[i][1]);
        i++;
    }
    wait_and_close_pipes(data, data->count_cmd);
}

void close_pipes(t_data *data)
{
    int i = 0;
    while (i < data->count_cmd - 1)
	{
        close(data->pipesfd[i][0]);
        close(data->pipesfd[i][1]);
        i++;
    }
    free(data->pipesfd);
}
