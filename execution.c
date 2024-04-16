/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 11:14:25 by jasnguye          #+#    #+#             */
/*   Updated: 2024/04/16 14:03:41 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include "Libft/libft.h"

void execution(t_data *data/* , t_command *cmd */)
{
	char *const *env = NULL;
	if(check_valid_command(data) != 1)
	{
		ft_printf("Not a valid command.\n");
	}
	else
	{
		ft_printf("Valid command.\n");
		t_command *cmd = data->commands;
		char *path = NULL;
		char *command = NULL;
		char *option = NULL;
		path = cmd->path;
		command = cmd->command; 

		printf("command: %s\n", cmd->command);
		printf("path: %s\n", path);
		if(data->commands != NULL && data->commands->option != NULL)//check if option exists
		{	
			printf("option is: %s\n", data->commands->option->value);
			option = data->commands->option->value;
		}
		char *const argv[] = {command, option, NULL};
	
		if(fork() == 0)//child process
		{
			
 			execve(path, argv, env); // Execute the command
       	 	perror("execve"); //in case of an error
		}
		else //this is the parent process
		{
			wait(NULL);
		}
		free(data->command_list);
	}
	
}


