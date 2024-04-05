/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 14:35:53 by jasnguye          #+#    #+#             */
/*   Updated: 2024/04/05 16:35:18 by jasnguye         ###   ########.fr       */
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
void check_valid_command(t_data *data)
{
	t_token *current = data->token_list;
	const char *executable_filename = "/bin/ls";
	char *path = getenv("PATH");
    if (!path)  // PATH variable not set
	{
        ft_printf("Path variable not set.\n");
    }
	while(current != NULL)
	{
		if(current->type == T_COMMAND)//only true for first token and token after pipe
		{
			if(ft_strcmp(current->value, "cd") == 0)// checks if it's a build-in command like cd, need to include others as well
			{
				ft_printf("Is a valid build-in command.\n");
			}
			else //if it's not
			{
				if(access(executable_filename, X_OK) == 0) //checks if it's an executable file(X_OK) in the path
				{
					ft_printf("Is a valid executable file in the path\n");
				}
				else 
				{
					ft_printf("Not a valid command.\n");//not a valid command at all
				}
			}
		
		}
		current = current->next;
	}



}
void parsing(t_data *data)
{
	check_valid_command(data);
}