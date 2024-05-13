/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 15:49:05 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/13 10:54:57 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

void setup_io_channels(int *io)
{
    if (io[0] != STDIN_FILENO) {
        dup2(io[0], STDIN_FILENO);
        close(io[0]);
    }
    if (io[1] != STDOUT_FILENO) {
        dup2(io[1], STDOUT_FILENO);
        close(io[1]);
    }
}

char **create_argv(t_command *cmd)
{
    int argc = count_arguments(cmd);
    char **argv = malloc(sizeof(char *) * (argc + 1));
    if (!argv) {
        perror("Memory allocation failed for argv");
        return NULL;
    }

    int i = 0;
    argv[i++] = cmd->command;
    if (cmd->option != NULL) {
        argv[i++] = cmd->option->value;
    }

    t_token *arg = cmd->argv;
    while (arg) {
        argv[i++] = arg->value;
        arg = arg->next;
    }
    argv[i] = NULL;

    return argv;
}
int count_arguments(t_command *cmd)
{
    int argc = 1; // Count the command itself
    if (cmd->option != NULL) {
        argc++;
    }
    t_token *arg = cmd->argv;
    while (arg) {
        argc++;
        arg = arg->next;
    }
    return argc;
}


int validate_io_channels(int *io)
{
    if (io[0] < 0 || io[1] < 0) {
        fprintf(stderr, "Error: Invalid IO channels determined.\n");
        return 0;
    }
    return 1;
}
int validate_command(t_command *cmd)
{
    if (!cmd->path || access(cmd->path, X_OK) != 0) {
        perror("Invalid command path or command is not executable");
        return 0;
    }
    return 1;
}
