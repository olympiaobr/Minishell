/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 18:30:22 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/06 16:10:43 by jasnguye         ###   ########.fr       */
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
void close_pipes(t_data *data)
{
    int i = 0;
    while (i < data->count_cmd - 1) {

        if (data->pipesfd[i]) {
            close(data->pipesfd[i][0]);
            close(data->pipesfd[i][1]);
            free(data->pipesfd[i]);
        }
        i++;
    }
    free(data->pipesfd);
    data->pipesfd = NULL;
}

int create_pipes(t_data *data)
{
    int num_pipes = data->count_cmd - 1;
    if (num_pipes <= 0)
    {
        printf("No pipes needed.\n");
        return 0;
    }
    printf("Number of pipes to create: %d\n", num_pipes);
    data->pipesfd = malloc(num_pipes * sizeof(int[2]));
    if (!data->pipesfd)
    {
        perror("Failed to allocate memory for pipe file descriptor arrays");
        return -1;
    }
    int i = 0;
    while (i < num_pipes)
    {
        data->pipesfd[i] = malloc(2 * sizeof(int));
        if (!data->pipesfd[i])
        {
            perror("Failed to allocate memory for individual pipes");
            close_pipes(data);
            return -1;
        }

        if (pipe(data->pipesfd[i]) == -1)
        {
            perror("Failed to create a pipe");
            close_pipes(data);
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
    while (i < data->count_cmd - 1)
    {
        close(data->pipesfd[i][0]);
        close(data->pipesfd[i][1]);
        i++;
    }
}

char **prepare_command_args(t_command *cmd)
{
    int argc = 1;  // Start with 1 for the command itself
    t_token *arg;

    if (cmd->option != NULL)
        argc++;  // Add one if there's an option

    // Count the arguments
    arg = cmd->argv;
    while (arg != NULL)
    {
        argc++;
        arg = arg->next;
    }
    // Allocate space for arguments + NULL terminator
    char **args = malloc(sizeof(char *) * (argc + 1));
    if (!args)
    {
        perror("Memory allocation failed for argv");
        exit(EXIT_FAILURE);
    }
    // Fill args array
    int i = 0;
    args[i++] = cmd->command;  // Set the command
    if (cmd->option != NULL)
    {
        args[i++] = cmd->option->value;
    }
    // Set each argument's value
    arg = cmd->argv;
    while (arg != NULL)
    {
        args[i++] = arg->value;
        arg = arg->next;
    }
    args[i] = NULL;
    return args;
}

char *resolve_command_path(char *command, t_data *data)
{
    if (command[0] == '/' || (command[0] == '.' && command[1] == '/'))
        return (ft_strdup(command));
    char *path_env = cust_getenv("PATH", data);
    if (!path_env)
        return NULL;
    char *path = ft_strdup(path_env);
    if (!path)
        return NULL;
    char *token = custom_strtok(path, ":");
    char full_path[1024];
    while (token)
    {
        ft_strcpy(full_path, token);
        ft_strcat(full_path, "/");
        ft_strcat(full_path, command);
        if (access(full_path, X_OK) == 0)
        {
            char *resolved_path = ft_strdup(full_path);
            free(path);
            return resolved_path;
        }
        token = custom_strtok(NULL, ":");
    }
    free(path);
    return NULL;
}

void execute_pipeline(t_data *data, t_command *cmd)
{
    int j = 0;
    int io[2];
    int i = 0;

    while (cmd != NULL && i < data->count_cmd)
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        { // Child process
            // Determine I/O channels for the current command
            determine_io_channels(data, j, io);
            // Setup input from previous command if not the first command
            if (j != 0)
            {
                dup2(io[0], STDIN_FILENO);
                close(io[0]); // Close the read end of the previous pipe after duping
            }
            // Setup output to next command if not the last command
            if (cmd->next != NULL) {
                dup2(io[1], STDOUT_FILENO);
                close(io[1]); // Close the write end of the current pipe after duping
            }
            char **args = prepare_command_args(cmd);
            char *cmd_path = resolve_command_path(args[0], data);
            execve(cmd_path, args, data->env);
            perror("execve");
            exit(EXIT_FAILURE);
        }
        // In parent, close the ends of the pipe this child used
        if (j < data->count_cmd - 1)
        {
            if (j != 0)
            {
                close(data->pipesfd[j - 1][0]); // Close the read end of the previous pipe
            }
            if (cmd->next != NULL)
            {
                close(data->pipesfd[j][1]); // Close the write end of the current pipe
            }
            j++;
        }
        cmd = cmd->next;
        i++;
    }
    wait_and_close_pipes(data, data->count_cmd);
}

