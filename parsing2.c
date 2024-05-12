/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 21:28:51 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/11 21:28:52 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

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
    return (0);
}

char *init_path_vars(t_data *data, char *cwd)
{
    char *path;
    char *path_copy;

	path = cust_getenv("PATH", data);
    if (!path)
        path = "";
    path_copy = ft_strdup(path);
    if (!path_copy)
        return NULL;
    if (getcwd(cwd, 1024) == NULL)
    {
        free(path_copy);
        return NULL;
    }
    return path_copy;
}

int check_builtin_command(t_token *current)
{
    if (check_builtin(current->value))
        return 1;
    return 0;
}
int check_command_path(t_token *current, char *cwd, t_data *data)
{
    char full_path[1024];

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
                free(data->commands->path);
            data->commands->path = ft_strdup(full_path);
            return 1;
        }
    }
    return 0;
}


int find_command(t_token *current, char *dir, t_data *data)
{
    char full_path[1024];

    if (find_command_path(current->value, dir, full_path))
    {
        if (data->commands->path)
            free(data->commands->path);
        data->commands->path = ft_strdup(full_path);
        return 1;
    }
    return 0;
}

static int process_command(t_token *current, char *cwd, char *dir, t_data *data)
{
    int command_result;

    command_result = check_builtin_command(current);
    if (command_result == 0)
        command_result = check_command_path(current, cwd, data);
    if (command_result == 0)
        command_result = find_command(current, dir, data);
    return command_result;
}
int check_valid_command(t_data *data)
{
    t_token *current;
    char cwd[1024];
    char *path_copy;
    char *dir;
    int result = -1;

    current = data->token_list;
    path_copy = init_path_vars(data, cwd);
    if (path_copy == NULL)
        return -1;
    dir = custom_strtok(path_copy, ":");
    while (current)
    {
        if (current->type == T_COMMAND)
        {
            if (process_command(current, cwd, dir, data))
            {
                free(path_copy);
                return 1;
            }
        }
        current = current->next;
    }
    free(path_copy);
    return result;
}





