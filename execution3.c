/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 18:30:22 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/04 18:30:24 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include "Libft/libft.h"

int count_commands(t_data *data)
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
    return count;
}

int create_pipes(t_data *data)
{
    int num_pipes = data->count_cmd - 1;
    int i = 0;
    if (num_pipes <= 0)
        return 0;

    data->pipesfd = malloc(num_pipes * sizeof(int[2]));
    if (!data->pipesfd)
	{
        perror("Failed to allocate memory for pipes");
        return -1;
    }
    while (i < num_pipes)
	{
        data->pipesfd[i] = malloc(2 * sizeof(int));
        if (!data->pipesfd[i])
		{
            perror("Failed to allocate memory for pipe");
            int j = 0;
            while (j < i)
			{
                free(data->pipesfd[j]);
                j++;
            }
            free(data->pipesfd);
            data->pipesfd = NULL;
            return -1;
        }
        if (pipe(data->pipesfd[i]) == -1)
		{
            perror("Failed to create a pipe");
            int j = 0;
            while (j <= i) {
                close(data->pipesfd[j][0]);
                close(data->pipesfd[j][1]);
                free(data->pipesfd[j]);
                j++;
            }
            free(data->pipesfd);
            data->pipesfd = NULL;
            return -1;
        }
        i++;
    }
    return 0;
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
