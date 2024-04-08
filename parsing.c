/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 14:35:53 by jasnguye          #+#    #+#             */
/*   Updated: 2024/04/08 15:47:41 by jasnguye         ###   ########.fr       */
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

char *ft_strcpy(char *dest, const char *src)
{
	int i = 0;
	while(src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return(dest);
}
char *ft_strcat(char *dest, char *src)
{
	int i = 0;
	int j = 0;
	while(dest[i] != '\0')
	{
		i++;
	}
	while(src[j] != '\0')
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return(dest);
}

char *custom_strtok(char *str, const char *delim) {
    static char *last = NULL; // pointer to the position in the string where the last token was found

    // if str is NULL, continue tokenizing from the last position
    if (str == NULL) 
	{
        str = last;
    }
	if (str == NULL || *str == '\0') 
	{
        last = NULL;
        return NULL;
    }

    str += strspn(str, delim);// skip leading delimiters
    // if the remaining string is empty, return NULL
    if (*str == '\0') 
	{
        last = NULL;
        return NULL;
    }
    // find end of the token
    char *token_end = str + strcspn(str, delim);

    // if a delimiter is found, replace it with a null terminator
    if (*token_end != '\0') 
	{
        *token_end = '\0';
        last = token_end + 1; // update the last pointer to point to the next character after the delimiter
    } 
	else 
	{
        last = NULL; // no more tokens left
    }

    return str;
}


void check_valid_command(t_data *data)
{
	t_token *current = data->token_list;
	char *path = getenv("PATH");
	char *path_copy = ft_strdup(path);
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
    			printf("path: %s\n", path_copy);
   				printf("command token: %s\n", current->value);
    			char full_path[1024];

    			full_path[0] = '\0';	// Initialize full_path to an empty string

    			char *dir = custom_strtok(path_copy, ":");// Iterate through each directory in PATH
    		
    			while (dir != NULL)
    			{
       			 	// Concatenate the directory and command
					
        			ft_strcpy(full_path, dir);
       	 			ft_strcat(full_path, "/");
       	 			ft_strcat(full_path, current->value);
        			
        			if (access(full_path, X_OK) == 0)// Check if the file exists and is executable
        			{
            			ft_printf("Is a valid executable file in the path\n");
            			break;
        			}
        			// Move to the next directory
        			dir = custom_strtok(NULL, ":");
				}
				//does not work properly yet
				if (full_path[0] == '\0')
    			{
        		ft_printf("Not a valid command.\n");
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