/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 11:14:25 by jasnguye          #+#    #+#             */
/*   Updated: 2024/04/16 18:01:19 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include "Libft/libft.h"

void execute_external_command(t_data *data, t_command *cmd)
{
    (void)data;

    char *path = cmd->path;
    char *command = cmd->command;
    char *option = NULL;
    char *arguments = NULL;

    if (cmd->option != NULL)
	{
        option = cmd->option->value;
        printf("option is: %s\n", option);
        arguments = option;
    }
    if (cmd->argv != NULL)
	{
        arguments = cmd->argv->value;
        printf("argument is: %s\n", arguments);
    }

    char *const argv[] = {command, arguments, NULL};
    char *const *env = NULL;

    if (fork() == 0)
	{ // child process
        execve(path, argv, env);
        perror("execve");
        exit(EXIT_FAILURE);
    }
	else
	{ // parent process
        wait(NULL);
    }
}

void execution(t_data *data)
{
    if (check_valid_command(data) != 1)
	{
        ft_printf("Not a valid command.\n");
    }
	else
	{
        ft_printf("Valid command.\n");
        t_command *cmd = data->commands;
        while (cmd != NULL) {
            if (is_builtin(cmd->command))
			{
                ft_printf("Executing built-in command: %s\n", cmd->command);
                if (execute_builtin(cmd, data) == -1)
				{
                    ft_printf("Error executing built-in command.\n");
                }
            }
			else
			{
                execute_external_command(data, cmd);
            }
            cmd = cmd->next;
        }
    }
}


