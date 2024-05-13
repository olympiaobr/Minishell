/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution9.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:04:54 by jasnguye          #+#    #+#             */
/*   Updated: 2024/05/13 12:12:48 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include "Libft/libft.h"

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
            //printf("Process %d exited with status: %d\n", pid, current_status);
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