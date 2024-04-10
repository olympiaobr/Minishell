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

int init_env(t_data *data, char **env){
    int i;

    i = 0;
    while (env[i])
        i++;
    data->env = (char **)malloc(sizeof(char *) * (i + 1));
    if (!data->env) {
        perror("Error allocating memory for env");
        return (EXIT_FAILURE);
    }
    i = 0;
    while (env[i])
    {
        data->env[i] = strdup(env[i]);
        if (!data->env[i])
        {
            perror("Error duplicating env variable");
            return (EXIT_FAILURE);
        }
        i++;
    }
    data->env[i] = NULL;
    return (EXIT_SUCCESS);
}

void process_environment_vars(t_data *data)
{
    int path_found = 0;
    int i = 0;

    while (data->env[i])
    {
        if (strncmp(data->env[i], "PATH=", 5) == 0)
        {
            data->path = strdup(data->env[i] + 5);
            path_found = 1;
            break;
        }
        i++;
    }
    if (!path_found)
    {
        perror("Error: PATH variable not found.");
        exit(EXIT_FAILURE);
    }
}

void init_environment_paths(t_data *data)
{
    int     i;

    if (!data->path)
    {
        perror("Error: PATH variable not initialized.");
        exit(EXIT_FAILURE);
    }
    data->path_dirs = ft_split(data->path, ':');
    if (!data->path_dirs)
    {
        perror("Error allocating memory for path directories");
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
        free(temp);
        exit(EXIT_FAILURE);
    }
    free(temp);
}
t_data *init_data(char **envp)
{
    t_data *data = (t_data *)malloc(sizeof(t_data));
    if (data == NULL) {
        perror("Error: Not enough memory to create main data structure");
        exit(EXIT_FAILURE);
    }
    ft_memset(data, 0, sizeof(t_data));

    init_env(data, envp);
    process_environment_vars(data, envp);
    init_environment_paths(data);

    //data->std_input_fd = STDIN_FILENO;
    //data->std_output_fd = STDOUT_FILENO;
    //data->exit_status = 0;
    return (data);
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

void free_all(t_data *data)
{
    int i;

    free(data->path);
    data->path = NULL;
    if (data->env)
    {
        i = 0;
        while (data->env[i])
        {
            free(data->env[i]);
            data->env[i] = NULL;
            i++;
        }
        free(data->env);
        data->env = NULL;
    }
   //   free_command_list
  //  free_tokens
    free_path_dirs(data);
}

void resolve_command(char **path_dirs, char *cmd, int *cmd_found)
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
    resolve_command(path_dirs + 1, cmd, cmd_found);
}

void resolve_command(t_data *data, char *cmd)
{
    data->cmd_found = 0;
    resolve_command(data->path_dirs, cmd, &data->cmd_found);
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
        	return 1;
		}
		return check_command_in_path(data->env_paths, command);
}
