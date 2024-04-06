/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 14:35:53 by jasnguye          #+#    #+#             */
/*   Updated: 2024/04/06 15:57:32 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

int ft_strcmp(char *s1, char *s2)
{
	int i = 0;
	while(s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
	{
		i++;
	}
	return(s1[i] - s2[i]);
}
/* void check_valid_command(t_data *data)
{
	t_token *current = data->token_list;
	char *path = getenv("PATH");
    if (!path)  // PATH variable not set
	{
        ft_printf("Path variable not set.\n");
    }
	
	while(current != NULL)
	{
		if(current->type == T_COMMAND)//only true for first token and token after pipe
		{
			if(ft_strcmp(current->value, "cd") == 0 || ft_strcmp(current->value, "echo") == 0 
			|| ft_strcmp(current->value, "pwd") == 0 ||ft_strcmp(current->value, "export") == 0 
			||ft_strcmp(current->value, "unset") == 0 ||ft_strcmp(current->value, "env") == 0
			||ft_strcmp(current->value, "exit") == 0)
			{
				ft_printf("Is a valid build-in command.\n");
			}
			else // If it's not a built-in command
			{
    			printf("path: %s\n", path);
   				printf("command token: %s\n", current->value);
    			char full_path[1024];

    		// Initialize full_path to an empty string
    			full_path[0] = '\0';

    		// Iterate through each directory in PATH
    			char *dir = strtok(path, ":");
    		while (dir != NULL)
    		{
       		 // Concatenate the directory and command
        	strcpy(full_path, dir);
       	 	strcat(full_path, "/");
       	 	strcat(full_path, current->value);

        	// Check if the file exists and is executable
        	if (access(full_path, X_OK) == 0)
        	{
            	ft_printf("Is a valid executable file in the path\n");
            	break;
        	}

        	// Move to the next directory
        	dir = strtok(NULL, ":");
	}

    // If full_path is still empty, the command was not found in any directory
    if (full_path[0] == '\0')
    {
        ft_printf("Not a valid command.\n");
    }
			}

		
		}
		current = current->next;
	}
} */

void parsing(t_data *data)
{
	check_valid_command(data);
}