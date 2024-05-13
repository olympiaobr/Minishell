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
    int count;
    int in_command;
    t_token *current;

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
    return count;
}
void close_pipes(t_data *data)
{
    int i;

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

int allocate_and_create_pipes(t_data *data, int num_pipes)
{
    int i;

    i = 0;
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
int create_pipes(t_data *data)
{
    int num_pipes;
    int result;

    num_pipes = data->count_cmd - 1;
    if (num_pipes <= 0)
    {
        printf("No pipes needed.\n");
        return 0;
    }
    printf("Number of pipes to create: %d\n", num_pipes);
    data->pipesfd = malloc(num_pipes * sizeof(int *));
    if (!data->pipesfd)
    {
        perror("Failed to allocate memory for pipe file descriptor arrays");
        return -1;
    }
    result = allocate_and_create_pipes(data, num_pipes);
    if (result != 0)
    {
        return result;
    }
    return 0;
}

void close_all_pipes(t_data *data)
{
    int index;

    index = 0;
    while (index < data->count_cmd - 1)
    {
        close(data->pipesfd[index][0]);
        close(data->pipesfd[index][1]);
        index++;
    }
}

int wait_for_processes(int num_processes)
{
    pid_t pid;
    int status;
    int exit_status;
    int current_status;

    exit_status = 0;
    while (num_processes > 0)
    {
        pid = wait(&status);
        if (pid == -1)
        {
            perror("wait");
            continue;
        }
        if (WIFEXITED(status))
        {
            current_status = WEXITSTATUS(status);
            printf("Process %d exited with status: %d\n", pid, current_status);
            if (current_status > exit_status)
                exit_status = current_status;
        }
        num_processes--;
    }
    return exit_status;
}

void wait_and_close_pipes(t_data *data, int num_processes)
{
    int exit_status;

    exit_status = wait_for_processes(num_processes);
    data->exit_status = exit_status;
    close_all_pipes(data);
}


char **prepare_command_space(t_command *cmd)
{
    int argc;
    t_token *arg;
    char **args;

    argc = 1;
    arg = cmd->argv;

    if (cmd->option != NULL)
        argc++;
    while (arg != NULL)
    {
        argc++;
        arg = arg->next;
    }
    args = malloc(sizeof(char *) * (argc + 1));
    if (!args)
    {
        perror("Memory allocation failed for argv");
        exit(EXIT_FAILURE);
    }
    return args;
}

char **prepare_command_args(t_command *cmd)
{
    char **args;
    int i;
    t_token *arg;

    args = prepare_command_space(cmd);
    i = 0;
    arg = cmd->argv;
    args[i++] = cmd->command;
    if (cmd->option != NULL)
    {
        args[i++] = cmd->option->value;
    }
    while (arg != NULL)
    {
        args[i++] = arg->value;
        arg = arg->next;
    }
    args[i] = NULL;
    return args;
}

char *get_cmd_fullpath(char *command, char *path_env)
{
    char full_path[1024];
    char *token;

    token = custom_strtok(path_env, ":");
    while (token)
    {
        ft_strcpy(full_path, token);
        ft_strcat(full_path, "/");
        ft_strcat(full_path, command);
        if (access(full_path, X_OK) == 0)
        {
            return ft_strdup(full_path);
        }
        token = custom_strtok(NULL, ":");
    }
    return NULL;
}
char *resolve_command_path(char *command, t_data *data)
{
    char *path_env;
    char *resolved_path;
    char *path;

    if (command[0] == '/' || (command[0] == '.' && command[1] == '/'))
        return ft_strdup(command);
    path_env = cust_getenv("PATH", data);
    if (!path_env)
        return NULL;
    path = ft_strdup(path_env);
    if (!path)
        return NULL;
    resolved_path = get_cmd_fullpath(command, path);
    free(path);
    return resolved_path;
}


void setup_and_execute_command(t_data *data, t_command *cmd, int j, int *io)
{
    if (j != 0)
    {
        dup2(io[0], STDIN_FILENO);
        close(io[0]);
    }
    if (cmd->next != NULL)
    {
        dup2(io[1], STDOUT_FILENO);
        close(io[1]);
    }
    char **args = prepare_command_args(cmd);
    char *cmd_path = resolve_command_path(args[0], data);
    execve(cmd_path, args, data->env);
    perror("execve");
    exit(EXIT_FAILURE);
}

void cmd_process(t_data *data, t_command *cmd, int *j, int io[2])
{
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        determine_io_channels(data, *j, io);
        setup_and_execute_command(data, cmd, *j, io);
    }
}

void close_unused_pipes(t_data *data, int j)
{
    if (j != 0)
    {
        close(data->pipesfd[j - 1][0]);
    }
    close(data->pipesfd[j][1]);
}

void execute_pipeline(t_data *data, t_command *cmd)
{
    int j = 0;
    int i = 0;
    int io[2];

    while (cmd != NULL && i < data->count_cmd)
    {
        cmd_process(data, cmd, &j, io);
        if (j < data->count_cmd - 1 && cmd->next != NULL)
        {
            close_unused_pipes(data, j);
            j++;
        }
        cmd = cmd->next;
        i++;
    }
    wait_and_close_pipes(data, data->count_cmd);
}
