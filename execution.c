/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 11:14:25 by jasnguye          #+#    #+#             */
/*   Updated: 2024/04/15 17:03:57 by jasnguye         ###   ########.fr       */
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
		char *path = NULL;
		char *command = NULL;
		char *option = NULL;
		path = data->command_list->path;
		command = data->command_list->command; 
		free(data->command_list);
	
		if(data->commands->option != NULL)
		{	printf("option is: %s\n", data->commands->option->value);
			option = data->commands->option->value;//check if it's a valid string first
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
	}
	
}


