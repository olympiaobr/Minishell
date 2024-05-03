/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 15:39:27 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/03 16:43:47 by jasnguye         ###   ########.fr       */
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


int operators_setup(t_data *data)
{
    int flags;

    /* if (data->heredoc)
	{
        data->std_input_fd = open(data->heredoc_input, O_RDONLY);
        if (data->std_input_fd == -1)
		{
            perror("Failed to open heredoc input file");
            data->std_input_fd = STDIN_FILENO;
        }
    } 
	else*/ if (data->input_file)
	{
        data->std_input_fd = open(data->input_file, O_RDONLY);
        if (data->std_input_fd == -1)
		{
            perror("Failed to open input file");
            data->std_input_fd = STDIN_FILENO;
        }
    }
	else
	{
        data->std_input_fd = STDIN_FILENO;
    }
    // Handle output redirection setup
    if (data->output_file)
	{
        flags = O_WRONLY | O_CREAT;
        if (data->append)
		{
            flags |= O_APPEND;
        }
		else
		{
            flags |= O_TRUNC;
        }
        data->std_output_fd = open(data->output_file, flags, 0644);
        if (data->std_output_fd == -1)
		{
            perror("Failed to open output file");
            data->std_output_fd = STDOUT_FILENO;
        }
    }
	else
	{
        data->std_output_fd = STDOUT_FILENO;
    }

    return 0;
}


void determine_io_channels(t_data *data, int cmd_index, int io[2])
 {
    // determine input channel
    if (cmd_index == 0)
	{
        io[0] = data->std_input_fd;
    }
	else
	{
        io[0] = data->pipesfd[cmd_index - 1][0];
    }
    // determine output channel
    if (cmd_index == data->count_cmd - 1)
	{
        io[1] = data->std_output_fd;
    }
	else
	{
        io[1] = data->pipesfd[cmd_index][1];
    }
}

int handle_redirections(t_data *data, int cmd_index)
{
    int io_channels[2], backup_fds[2];

    backup_fds[0] = dup(STDIN_FILENO);
    backup_fds[1] = dup(STDOUT_FILENO);
    if (backup_fds[0] == -1 || backup_fds[1] == -1)
	{
        perror("Failed to backup file descriptors");
        return -1;
    }

    determine_io_channels(data, cmd_index, io_channels);
    if (io_channels[0] != STDIN_FILENO)
	{
        if (dup2(io_channels[0], STDIN_FILENO) == -1)
		{
            perror("Failed to redirect standard input");
            // Restore and close backups before exiting
            dup2(backup_fds[0], STDIN_FILENO);
            dup2(backup_fds[1], STDOUT_FILENO);
            close(backup_fds[0]);
            close(backup_fds[1]);
            return -1;
        }
    }
    if (io_channels[1] != STDOUT_FILENO) {
        if (dup2(io_channels[1], STDOUT_FILENO) == -1)
		{
            perror("Failed to redirect standard output");
            dup2(backup_fds[0], STDIN_FILENO);
            dup2(backup_fds[1], STDOUT_FILENO);
            close(backup_fds[0]);
            close(backup_fds[1]);
            return -1;
        }
    }

    dup2(backup_fds[0], STDIN_FILENO);
    dup2(backup_fds[1], STDOUT_FILENO);
    close(backup_fds[0]);
    close(backup_fds[1]);

    return 0;
}

void restore_redirections(t_data *data, int cmd_index, int io[2], int backup_fds[2])
{
    if (cmd_index > 0)
        close(data->pipesfd[cmd_index - 1][0]);
    if (cmd_index < data->count_cmd - 1)
        close(data->pipesfd[cmd_index][1]);

    if (io[0] != STDIN_FILENO && dup2(backup_fds[0], STDIN_FILENO) == -1)
        perror("Failed to restore standard input");
    if (io[1] != STDOUT_FILENO && dup2(backup_fds[1], STDOUT_FILENO) == -1)
        perror("Failed to restore standard output");

    if (io[0] > 2)
        close(io[0]);
    if (io[1] > 2)
        close(io[1]);

    close(backup_fds[0]);
    close(backup_fds[1]);
}

int execute_builtin(t_command *cmd, t_data *data)
{
    int backup_fds[2];
    int io_channels[2];

    // backup original file descriptors
    backup_fds[0] = dup(STDIN_FILENO);
    backup_fds[1] = dup(STDOUT_FILENO);
    if (backup_fds[0] == -1 || backup_fds[1] == -1)
	{
        perror("Failed to backup file descriptors");
        return -1;
    }
    // Setup redirections
    determine_io_channels(data, cmd->command_index, io_channels);
    if (io_channels[0] != STDIN_FILENO && dup2(io_channels[0], STDIN_FILENO) == -1)
	{
        perror("Failed to redirect standard input");
        return -1;
    }
    if (io_channels[1] != STDOUT_FILENO && dup2(io_channels[1], STDOUT_FILENO) == -1)
	{
        perror("Failed to redirect standard output");
        return -1;
    }
    int result = -1;
    if (ft_strcmp(cmd->command, "cd") == 0)
        result = cd_cmd(data, cmd);
    else if (ft_strcmp(cmd->command, "echo") == 0)
        result = echo_cmd(cmd);
    else if (ft_strcmp(cmd->command, "pwd") == 0)
        result = pwd_cmd();
    else if (ft_strcmp(cmd->command, "env") == 0)
        result = env_cmd(data);
    else if (ft_strcmp(cmd->command, "export") == 0)
        result = export_cmd(data, cmd);
    else if (ft_strcmp(cmd->command, "unset") == 0)
        result = unset_cmd(data, cmd);
    else if (ft_strcmp(cmd->command, "exit") == 0)
        result = exit_cmd(data, cmd);

    // restore original file descriptors
    dup2(backup_fds[0], STDIN_FILENO);
    dup2(backup_fds[1], STDOUT_FILENO);
    close(backup_fds[0]);
    close(backup_fds[1]);

    return result;
}




