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

int init_env(t_data *data, char **env)
{
    int i;

    i = 0;
    while (env[i])
        i++;
    data->env = (char **)malloc(sizeof(char *) * (i + 1));
    if (!data->env)
    {
        perror("Error allocating memory for env");
        return (EXIT_FAILURE);
    }
    i = 0;
    while (env[i])
    {
        data->env[i] = ft_strdup(env[i]);
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
void prepare_environment(t_data *data)
{
    int i;
    char *path_value = cust_getenv("PATH", data);
    if (!path_value)
    {
        perror("Error: PATH variable not found.");
        exit(EXIT_FAILURE);
    }
    data->path_dirs = ft_split(path_value, ':');
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
char *cust_strstr(const char *h, const char *n)
{
    const char *h;
    const char *n;

    if (*n == '\0')
    {
        return ((char *)h);
    }
    while (*h != '\0')
    {
        if (*h == *n)
        {
            h = h;
            n = n;
            while (*h != '\0' && *n != '\0' && *h == *n)
            {
                h++;
                n++;
            }
            if (*n == '\0')
            {
                return ((char *)h);
            }
        }
        h++;
    }
    return (NULL);
}

char *cust_getenv(const char *name, t_data *data)
{
    int i = 0;
    char *env_entry;
    char *found;

    while ((env_entry = data->env[i]) != NULL)
    {
        found = cust_strstr(env_entry, name);
        if (found && found[ft_strlen(name)] == '=' && (found == env_entry || *(found - 1) == '\0'))
        {
            return (found + ft_strlen(name) + 1);
        }
        i++;
    }
    return (NULL);
}

void initialize_shell_components(t_data *data)
{
    data->std_input_fd = STDIN_FILENO;
    data->std_output_fd = STDOUT_FILENO;
    data->user_input = NULL;
    data->token_list = NULL;
    data->command_list = NULL;
    data->current_token = NULL;
    data->std_input = NULL;
    data->std_output = NULL;
    data->input_file = NULL;
    data->output_file = NULL;
    data->append = 0;
    data->heredoc = 0;
    data->commands = NULL;
    data->create_new_command = 0;
    data->exit_status = 0;
}
t_data *init_data(char **envp)
{
    t_data *data = (t_data *)malloc(sizeof(t_data));
    if (data == NULL)
    {
        perror("Error: Not enough memory to create main data structure");
        exit(EXIT_FAILURE);
    }
    ft_memset(data, 0, sizeof(t_data));
    if (init_env(data, envp) == EXIT_FAILURE)
    {
        free(data);
        return NULL;
    }
    prepare_environment(data);
    initialize_shell_components(data);
    return (data);
}

void free_all(t_data *data)
{
    int i = 0;

    // free environment var
    while (data->env && data->env[i])
    {
        free(data->env[i]);
        data->env[i] = NULL;
        i++;
    }
    free(data->env);
    data->env = NULL;

    // free path directories
    if (data->path_dirs)
    {
        i = 0;
        while (data->path_dirs[i])
        {
            free(data->path_dirs[i]);
            data->path_dirs[i] = NULL;
            i++;
        }
        free(data->path_dirs);
        data->path_dirs = NULL;
    }
    free_tokens(data);
    free_commands(data->commands);
    free(data->input_file);
    free(data->output_file);
}
/*
to be figured out
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
*/
