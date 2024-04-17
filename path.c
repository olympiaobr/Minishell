/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 14:36:04 by olobresh          #+#    #+#             */
/*   Updated: 2024/04/15 14:07:26 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

int init_env(t_data *data, char **env) {
    t_env_var *last = NULL;
    for (int i = 0; env[i]; i++) {
        char *key = strdup(env[i]);
        if (!key) {
            perror("Failed to duplicate environment key");
            return EXIT_FAILURE;
        }
        char *separator = strchr(key, '=');
        if (separator) {
            *separator = '\0';  // Null-terminate the key
            separator++;        // Move to the beginning of the value
        }

        t_env_var *new_var = malloc(sizeof(t_env_var));
        if (!new_var) {
            free(key);
            perror("Failed to allocate memory for new environment variable");
            return EXIT_FAILURE;
        }
        new_var->key = key;
        new_var->value = strdup(separator ? separator : "");
        new_var->next = NULL;

        if (last) {
            last->next = new_var;
        } else {
            data->env_head = new_var;  // Assuming `env_head` is the start of your linked list in `t_data`
        }
        last = new_var;
    }
    return EXIT_SUCCESS;
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
void prepare_environment(t_data *data) {
    char *path_value = find_env_var(data->env_head, "PATH");
    if (!path_value) {
        perror("Error: PATH variable not found.");
        exit(EXIT_FAILURE);
    }
    data->path_dirs = ft_split(path_value, ':');
    if (!data->path_dirs) {
        perror("Error allocating memory for path directories");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; data->path_dirs[i]; i++) {
        append_slash(&(data->path_dirs[i]));
    }
}
char *cust_strstr(const char *haystack, const char *needle)
{
    const char *h_ptr;
    const char *n_ptr;

    if (*needle == '\0')
    {
        return ((char *)haystack);
    }

    while (*haystack != '\0')
    {
        if (*haystack == *needle)
        {
            h_ptr = haystack;
            n_ptr = needle;
            while (*h_ptr != '\0' && *n_ptr != '\0' && *h_ptr == *n_ptr)
            {
                h_ptr++;
                n_ptr++;
            }
            if (*n_ptr == '\0')
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
    data->env_head = NULL;
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
    data->input_file = NULL;
    free(data->output_file);
    data->output_file = NULL;
}

/*


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
