/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 15:39:27 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/13 12:25:26 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include "Libft/libft.h"

void determine_io_channels(t_data *data, int cmd_index, int io[2])
{
    int default_input;
    int default_output;

	default_input = STDIN_FILENO;
	default_output = STDOUT_FILENO;
    io[0] = -1;
    io[1] = -1;
	set_input_channel(data, cmd_index, default_input, io);
    if (cmd_index == data->count_cmd - 1)
    {
        if (data->std_output_fd > 0)
            io[1] = data->std_output_fd;
        else
            io[1] = default_output;
    }
    else if (data->pipesfd && cmd_index < data->count_cmd - 1)
        io[1] = data->pipesfd[cmd_index][1];
    if (io[0] < 0 || io[1] < 0)
		ft_printf("Error: Invalid IO channels for cmd_ind %d.\n", cmd_index);
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

int call_builtin(t_command *cmd, t_data *data)
{
    if (ft_strcmp(cmd->command, "cd") == 0)
        return (cd_cmd(data, cmd));
    else if (ft_strcmp(cmd->command, "echo") == 0)
        return (echo_cmd(cmd));
    else if (ft_strcmp(cmd->command, "pwd") == 0)
        return (pwd_cmd());
    else if (ft_strcmp(cmd->command, "env") == 0)
        return (env_cmd(data));
    else if (ft_strcmp(cmd->command, "export") == 0)
        return (export_cmd(data, cmd));
    else if (ft_strcmp(cmd->command, "unset") == 0)
        return (unset_cmd(data, cmd));
    else if (ft_strcmp(cmd->command, "exit") == 0)
        return (exit_cmd(data, cmd));
    return (-1);
}

int handle_redirections(t_data *data, int cmd_index, int *io_channels)
{
    determine_io_channels(data, cmd_index, io_channels);

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
    return 0;
}
int execute_builtin(t_command *cmd, t_data *data)
{
    int backup_fds[2];
    int io_channels[2];
    int result;

    backup_fds[0] = dup(STDIN_FILENO);
    backup_fds[1] = dup(STDOUT_FILENO);
    if (backup_fds[0] == -1 || backup_fds[1] == -1)
	{
        perror("Failed to backup file descriptors");
        return -1;
    }
    if (handle_redirections(data, cmd->command_index, io_channels) == -1)
        return (-1);
    result = call_builtin(cmd, data);
    restore_redirections(data, cmd->command_index, io_channels, backup_fds);
    return (result);
}

