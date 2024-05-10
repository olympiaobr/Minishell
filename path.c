/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 14:36:04 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/10 13:31:19 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

int copy_env_vars(t_data *data, char **env, int count)
{
    int j;

    j = 0;
    while (j < count)
    {
        data->env[j] = ft_strdup(env[j]);
        if (!data->env[j])
        {
            while (j > 0)
            {
                j--;
                free(data->env[j]);
            }
            free(data->env);
            perror("Error duplicating env variable");
            return (EXIT_FAILURE);
        }
        j++;
    }
    data->env[count] = NULL;
    return (EXIT_SUCCESS);
}

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
    if (copy_env_vars(data, env, i) == EXIT_FAILURE)
    {
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
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
void prepare_environment(t_data *data)
{
    int i;
    char *path_value;

    path_value = cust_getenv("PATH", data);
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
int match_found(const char *haystack, const char *needle)
{
    while (*haystack && *needle && *haystack == *needle)
    {
        haystack++;
        needle++;
    }
    return (*needle == '\0');
}

char *cust_strstr(const char *haystack, const char *needle)
{
    if (*needle == '\0')
    {
        return ((char *)haystack);
    }

    while (*haystack != '\0')
    {
        if (*haystack == *needle)
        {
            if (match_found(haystack, needle))
            {
                return ((char *)haystack);
            }
        }
        haystack++;
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
    data->max_env_size = 0;
	data->heredoc_input = NULL;
    data->original_stdout = -1;
    data->original_stdin = -1;
    data->pipesfd = NULL;
}

t_data *init_data(char **envp)
{
    t_data *data;

    data = (t_data *)malloc(sizeof(t_data));
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
	data->exit_status = 0;
    initialize_shell_components(data);
    return (data);
}
