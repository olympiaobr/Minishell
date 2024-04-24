/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 14:35:53 by jasnguye          #+#    #+#             */
/*   Updated: 2024/04/24 12:19:44 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
	{
        s1++;
        s2++;
    }
    return (*(const unsigned char*)s1 - *(const unsigned char*)s2);
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
char *ft_strcat(char *dest, const char *src)
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
size_t ft_strspn(const char *str, const char *delim)
{
    const char *p;
    const char *s;
    s = str;
    while (*s != '\0')
    {
        p = delim;
        while (*p != '\0' && *p != *s)
            p++;
        if (*p == '\0')
            break;
        s++;
    }
    return (s - str);
}

size_t ft_strcspn(const char *str, const char *reject)
{
    const char *s;
    const char *r;
    s = str;
    while (*s != '\0')
    {
        r = reject;
        while (*r != '\0')
        {
            if (*s == *r)
                return (s - str);
            r++;
        }
        s++;
    }
    return (s - str);
}


char *custom_strtok(char *str, const char *delim)
{
    static char *last;
    char *end;

    if (str == NULL)
        str = last;
    if (str == NULL || *str == '\0')
        return NULL;
    str += ft_strspn(str, delim);
    if (*str == '\0')
        return (last = NULL);
    end = str + ft_strcspn(str, delim);
    if (*end != '\0')
        *end++ = '\0';
    last = end;
    return str;
}
int find_command_path(const char *command, char *dir, char *full_path)
{
    while (dir)
	{
        ft_strcpy(full_path, dir);
        ft_strcat(full_path, "/");
        ft_strcat(full_path, command);
        if (access(full_path, X_OK) == 0)
		{
            return 1;
        }
        dir = custom_strtok(NULL, ":");
    }
    return 0;
}

int check_valid_command(t_data *data)
{
    t_token *current = data->token_list;
    char *path = getenv("PATH");
    char *path_copy;
    char *dir;
    char full_path[1024];
    char cwd[1024];
    int result = -1;

    if (!path)
		return -1;
    path_copy = ft_strdup(path);
    if (!path_copy)
		return -1;
    if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
        free(path_copy);
        return -1;
    }
    dir = custom_strtok(path_copy, ":");
    while (current) {
        if (current->type == T_COMMAND)
		{
            if (check_builtin(current->value))
			{
                result = 1;
                break;
            }
            if (current->value[0] == '/' || ft_strncmp(current->value, "./", 2) == 0 || ft_strncmp(current->value, "../", 3) == 0)
			{
                if (current->value[0] != '/')
				{
                    ft_strcpy(full_path, cwd);
                    if (full_path[ft_strlen(full_path) - 1] != '/')
                        ft_strcat(full_path, "/");
                    ft_strcat(full_path, current->value);
                }
				else
				{
                    ft_strcpy(full_path, current->value);
                }
                if (access(full_path, X_OK) == 0)
				{
                    if (data->commands->path)
					{
                        free(data->commands->path);
                    }
                    data->commands->path = ft_strdup(full_path);
                    result = 1;
                    break;
                }
            }
            if (find_command_path(current->value, dir, full_path))
			{
                if (data->commands->path) {
                    free(data->commands->path);
                }
                data->commands->path = ft_strdup(full_path);
                result = 1;
                break;
            }
        }
        current = current->next;
    }
    free(path_copy);
    return result;
}










/*
void parsing(t_data *data)
{
 	if(check_valid_command(data) != 1)
	{
		ft_printf("Not a valid command.\n");
	}
	else
	{
		ft_printf("Valid command.\n");
	}
}
*/
