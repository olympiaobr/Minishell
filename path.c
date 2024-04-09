/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 14:36:04 by olobresh          #+#    #+#             */
/*   Updated: 2024/04/09 14:36:06 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

void init_environment_paths(t_data *data)
{
    char    *path_env;
    int     i;

    path_env = getenv("PATH");
    if (!path_env)
	{
        perror("Error: Unable to retrieve PATH environment variable.");
        exit(EXIT_FAILURE);
    }
    data->path_dirs = ft_split(path_env, ':');
    if (!data->path_dirs)
	{
        perror("Error: Memory allocation failed for PATH directories.");
        exit(EXIT_FAILURE);
    }
    i = 0;
    while (data->path_dirs[i])
	{
        append_slash(&(data->path_dirs[i]));
        i++;
    }
}

void append_slash(char **directory)
{
    char *temp;

    temp = *directory;
    *directory = ft_strjoin(temp, "/");
    if (!(*directory))
	{
        perror("Error appending slash to PATH directory");
        free(temp); // Ensure previously allocated directory is freed upon error.
        exit(EXIT_FAILURE);
    }
    free(temp); // Clean up the original directory string.
}

void free_path_dirs(t_data *data)
{
    int index = 0;
    while (data->path_dirs && data->path_dirs[index])
	{
        free(data->path_dirs[index]);
        index++;
    }
    free(data->path_dirs);
}
/*
void resolve_command_recursive(char **path_dirs, char *cmd, int *cmd_found)
{
    if (!path_dirs || !*path_dirs)
	{
        *cmd_found = 0;
        return;
    }

    char *cmd_path = ft_strjoin3(*path_dirs, "/", cmd);
    if (access(cmd_path, X_OK) == 0)
	{
        *cmd_found = 1;
        free(cmd_path);
        return;
    }
    free(cmd_path);
    resolve_command_recursive(path_dirs + 1, cmd, cmd_found);
}

void resolve_command(t_data *data, char *cmd)
{
    data->cmd_found = 0;
    resolve_command_recursive(data->path_dirs, cmd, &data->cmd_found);
}

int check_command_in_path(char **env_paths, const char *command)
{
    if (!*env_paths)
	{
        return 0;
    }
    char *full_path = ft_strjoin(*env_paths, "/");
    char *executable = ft_strjoin(full_path, command);
    free(full_path);

    if (access(executable, X_OK) == 0)
	{
        free(executable);
        return 1;
    }
    free(executable);
    return check_command_in_path(env_paths + 1, command);
}

int check_valid_command(t_data *data, const char *command)
{
    	if (strcmp(command, "cd") == 0 || strcmp(command, "echo") == 0 ||
        strcmp(command, "pwd") == 0 || strcmp(command, "export") == 0 ||
        strcmp(command, "unset") == 0 || strcmp(command, "env") == 0 ||
        strcmp(command, "exit") == 0)
		{
        	return 1; // True, it's a built-in command
		}
		return check_command_in_path(data->env_paths, command);
}
*/
