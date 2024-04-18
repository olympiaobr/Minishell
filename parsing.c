/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 14:35:53 by jasnguye          #+#    #+#             */
/*   Updated: 2024/04/16 14:26:01 by jasnguye         ###   ########.fr       */
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

int check_valid_command(t_data *data)
{
    t_token *current;
    int valid;
    int not_valid;
    char *path;
    char *path_copy;
    char *dir;
    char full_path[1024];

    current = data->token_list;
    valid = 1;
    not_valid = 1;
    data->command_list = malloc(sizeof(t_command));
    if (!data->command_list)
    {
        fprintf(stderr, "Allocation failed\n");
        return -1;
    }
    memset(data->command_list, 0, sizeof(t_command));

    path = getenv("PATH");
    if (!path)
    {
        free(data->command_list);
        return -1;
    }
    path_copy = strdup(path);
    if (!path_copy)
    {
        free(data->command_list);
        return -1;
    }

    dir = custom_strtok(path_copy, ":");
    while (current)
    {
        if (current->type == T_COMMAND)
        {
            if (!strcmp(current->value, "cd") || !strcmp(current->value, "echo") ||
                !strcmp(current->value, "pwd") || !strcmp(current->value, "export") ||
                !strcmp(current->value, "unset") || !strcmp(current->value, "env") ||
                !strcmp(current->value, "exit"))
            {
                valid = 1;
            }
            else if (current->value[0] == '/')
            {
                if (access(current->value, X_OK) == 0)
                {
                    data->commands->path = strdup(current->value);
                    valid = 1;
                }
                else
                {
                    not_valid = -1;
                }
            }
            else
            {
                while (dir)
                {
                    snprintf(full_path, sizeof(full_path), "%s/%s", dir, current->value);
                    if (access(full_path, X_OK) == 0)
                    {
                        data->commands->path = strdup(full_path);
                        valid = 1;
                        break;
                    }
                    dir = custom_strtok(NULL, ":");
                }
            }
        }
        if (!valid)
            not_valid = -1;
        current = current->next;
    }

    free(path_copy);
    if (not_valid == -1)
    {
        free(data->command_list);
        data->command_list = NULL;
        return -1;
    }
    return valid;
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
