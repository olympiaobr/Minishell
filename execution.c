/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 11:14:25 by jasnguye          #+#    #+#             */
/*   Updated: 2024/04/17 15:04:28 by jasnguye         ###   ########.fr       */
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
    char *argument1 = NULL;
    char *argument2 = NULL;
    int argc = 1;

    if (cmd->option != NULL)
    {
        option = cmd->option->value;
        printf("option is: %s\n", option);
        argc++;
    }
    if (cmd->argv != NULL)
    {
        argument1 = cmd->argv->value;
        printf("argument1 is: %s\n", argument1);
        argc++;
        if (cmd->argv->next != NULL && cmd->argv->next->value != NULL)
        {
            argument2 = cmd->argv->next->value;
            printf("argument2 is: %s\n", argument2);
            argc++;
        }
    }

    char **argv = malloc(sizeof(char *) * (argc + 1));
    if (!argv)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    argv[i++] = command;
    if (option != NULL)
    {
        argv[i++] = option;
    }
    if (argument1 != NULL)
    {
        argv[i++] = argument1;
    }
    if (argument2 != NULL)
    {
        argv[i++] = argument2;
    }
    argv[i] = NULL;

    if (fork() == 0)  // Child process
    {
        execve(path, argv, data->env);
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else  // Parent process
    {
        wait(NULL);
    }
    free(argv);
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


