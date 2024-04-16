/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 14:39:15 by olobresh          #+#    #+#             */
/*   Updated: 2024/04/15 14:39:17 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

void print_command_details(t_data *data)
{
    printf("Printing command details:\n");
    if (data->command_list == NULL)
	{
        printf("No commands to display.\n");
        return;
    }

    t_command *current_cmd = data->command_list;
    int index = 0;
    while (current_cmd != NULL)
	{
        printf("Command %d:\n", index + 1);
        printf("  Command: %s\n", current_cmd->command);
        if (is_builtin(current_cmd->command))
		{
            printf("  Type: Built-in Command\n");
        }
		else
		{
            printf("  Type: External Command\n");
            if
			(data->cmd_paths[index] != NULL)
			{
                printf("  Path: %s\n", data->cmd_paths[index]);
            }
			else
			{
                printf("  Path: Command path not found\n");
            }
        }
        t_token *arg = current_cmd->argv;
        if (arg != NULL)
		{
            printf("  Arguments: ");
            while (arg != NULL)
			{
                printf("%s ", arg->value);
                arg = arg->next;
            }
            printf("\n");
        }
        current_cmd = current_cmd->next;
        index++;
    }
}

int count_commands(t_token *tokens)
{
    int count = 1;
    t_token *current = tokens;
    while (current) {
        if (current->type == T_PIPE)
		{
            count++;
        }
        current = current->next;
    }
    return count;
}

void update_path_dirs(t_data *data)
{
    char *path_value = cust_getenv("PATH", data);
    if (!path_value)
	{
        perror("Error: PATH variable not found.");
        exit(EXIT_FAILURE);
    }
    char **tmp = ft_split(path_value, ':');
    if (!tmp) {
        perror("Error reallocating memory for updated path directories");
        exit(EXIT_FAILURE);
    }
    if (data->path_dirs)
	{
        char **to_free = data->path_dirs;
        while (*to_free)
		{
            free(*to_free);
            to_free++;
        }
        free(data->path_dirs);
    }

    data->path_dirs = tmp;
    char **path_iter = data->path_dirs;
    while (*path_iter)
	{
        append_slash(path_iter);
        path_iter++;
    }
}

char *find_executable_path(char *command, t_data *data)
{
    if (!command)
        return NULL;

    char *full_path;

    int i = 0;
    while (data->path_dirs[i] != NULL)
	{
        full_path = ft_strjoin(data->path_dirs[i], command);

        if (access(full_path, X_OK) == 0)
		{
            return full_path;
        }
        free(full_path);
        i++;
    }
    return NULL;
}
int is_builtin(const char *command)
{
    return (strcmp(command, "cd") == 0 || strcmp(command, "echo") == 0 ||
            strcmp(command, "pwd") == 0 || strcmp(command, "export") == 0 ||
            strcmp(command, "unset") == 0 || strcmp(command, "env") == 0 ||
            strcmp(command, "exit") == 0);
}

void process_commands_(t_data *data)
{
    t_command *last_cmd = NULL;
    t_token *current_token = data->token_list;
    int command_index = 0;

    while (current_token)
	{
        if (current_token->type == T_COMMAND)
		{
            char *command_path = NULL;
            if (is_builtin(current_token->value))
			{
                command_path = strdup(current_token->value);
            }
			else
			{
                command_path = find_executable_path(current_token->value, data);
                if (!command_path)
				{
                    fprintf(stderr, "Command not found: %s\n", current_token->value);
                    current_token = current_token->next;
                    continue;
                }
                data->cmd_paths[command_index] = strdup(command_path);
            }
            t_command *new_cmd = init_command(command_path);
            if (command_path)
				free(command_path);
            data->commands[command_index++] = new_cmd;
            if (!last_cmd) data->command_list = new_cmd;
            else last_cmd->next = new_cmd;
            last_cmd = new_cmd;
        }
        current_token = current_token->next;
    }
}

void initialize_command_processing(t_data *data)
{
    // PATH directories must be current before processing
    update_path_dirs(data);
    // Determine the number of commands to process
    int num_commands = count_commands(data->token_list);
    if (num_commands <= 0)
	{
        ft_printf("Error: No valid commands found.\n");
        return;
    }
    // alloc mem for commands and their executable paths
    data->commands = (t_command **)calloc(num_commands, sizeof(t_command *));
    if (!data->commands)
	{
        perror("Failed to allocate memory for commands");
        exit(EXIT_FAILURE);
    }
    data->cmd_paths = (char **)calloc(num_commands, sizeof(char *));
    if (!data->cmd_paths)
	{
        perror("Failed to allocate memory for command paths");
        free(data->commands);
        exit(EXIT_FAILURE);
    }
    process_commands_(data);
}
