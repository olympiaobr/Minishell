/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 11:14:25 by jasnguye          #+#    #+#             */
/*   Updated: 2024/04/17 15:04:28 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"
#include "Libft/libft.h"


// Helper function to count the number of arguments
int count_args(t_command *cmd) {
    int count = 1;  // Start with 1 for the command itself
    if (cmd->option != NULL) count++;
    for (t_token *arg = cmd->argv; arg != NULL; arg = arg->next) {
        count++;
    }
    return count;
}

// Function to build argv from a t_command
char **build_argv_from_cmd(t_command *cmd) {
    int argc = 1;
    for (t_token *arg = cmd->argv; arg != NULL; arg = arg->next) {
        argc++;
    }

    char **argv = malloc(sizeof(char*) * (argc + 1));  // +1 for NULL termination
    if (!argv) {
        perror("Memory allocation failed for argv");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    argv[i++] = cmd->command;
    for (t_token *arg = cmd->argv; arg != NULL; arg = arg->next) {
        argv[i++] = arg->value;
    }
    argv[i] = NULL;  // NULL terminate the array

    return argv;
}


void free_argv(char **argv) {
    free(argv);  // Assuming argv was allocated in one block
}

char **env_var_list_to_array(t_env_var *env_head) {
    // First, count the number of environment variables
    int count = 0;
    for (t_env_var *cur = env_head; cur != NULL; cur = cur->next) {
        count++;
    }

    // Allocate memory for the array (+1 for the NULL terminator)
    char **env_array = malloc((count + 1) * sizeof(char *));
    if (!env_array) {
        perror("Memory allocation failed for environment array");
        return NULL;
    }

    // Fill the array
    t_env_var *current = env_head;
    for (int i = 0; current != NULL; i++) {
        int len = strlen(current->key) + strlen(current->value) + 2;  // +2 for '=' and '\0'
        env_array[i] = malloc(len);
        if (!env_array[i]) {
            // Clean up previously allocated memory
            while (i > 0) {
                free(env_array[--i]);
            }
            free(env_array);
            perror("Memory allocation failed for environment string");
            return NULL;
        }
        sprintf(env_array[i], "%s=%s", current->key, current->value);
        current = current->next;
    }
    env_array[count] = NULL;  // NULL terminate the array

    return env_array;
}

void free_env_array(char **env_array) {
    if (env_array == NULL) return;
    for (int i = 0; env_array[i] != NULL; i++) {
        free(env_array[i]);
    }
    free(env_array);
}

void execute_external_command(t_data *data, t_command *cmd) {
    // Build argv and env arrays
    char **argv = build_argv_from_cmd(cmd);
    char **env = env_var_list_to_array(data->env_head);

    // Debug: Print command and environment
    printf("Executing command: %s\nArguments:\n", cmd->path);
    for (int i = 0; argv && argv[i]; i++) {
        printf("  argv[%d]: %s\n", i, argv[i]);
    }
    printf("Environment:\n");
    for (int i = 0; env && env[i]; i++) {
        printf("  env[%d]: %s\n", i, env[i]);
    }

    // Execute the command
    pid_t pid = fork();
    if (pid == 0) {  // Child process
        execve(cmd->path, argv, env);
        perror("execve failed");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("fork failed");
    } else {  // Parent process
        int status;
        wait(&status);
        printf("Command exited with status %d\n", WEXITSTATUS(status));
    }

    // Cleanup
    free_argv(argv);
    free_env_array(env);
}




void execution(t_data *data)
{
    if (check_valid_command(data) != 1)
	{
        ft_printf("Not a valid command.\n");
    }
	else
	{
        ft_printf("Valid command.\n");
        t_command *cmd = data->commands;
        while (cmd != NULL) {
            if (is_builtin(cmd->command))
			{
                ft_printf("Executing built-in command: %s\n", cmd->command);
                if (execute_builtin(cmd, data) == -1)
				{
                    ft_printf("Error executing built-in command.\n");
                }
            }
			else
			{
                execute_external_command(data, cmd);
            }
            cmd = cmd->next;
        }
    }
}


