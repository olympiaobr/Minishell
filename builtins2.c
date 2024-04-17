/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:12:55 by olobresh          #+#    #+#             */
/*   Updated: 2024/04/16 13:12:59 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

int check_args(t_command *cmd)
{
    if (cmd->argc > 2)
    {
        fprintf(stderr, "cd: too many arguments\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


// Function to fetch an environment variable's value
char *get_env_var(char **envp, const char *name)
{
    size_t len;
    int i;

    len = ft_strlen(name);
    i = 0;
    while (envp[i] != NULL)
    {
        if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
        {
            return envp[i] + len + 1;
        }
        i++;
    }
    return NULL;
}

// update or add an environment variable if valid
int set_env_var(t_env_var **env_head, const char *name, const char *value) {
    if (!name || !value || name[0] == '\0' || strchr(name, '=') != NULL) {
        fprintf(stderr, "Invalid environment variable name\n");
        return -1;  // Invalid name
    }

    t_env_var *current = *env_head;
    t_env_var *last = NULL;

    // Check if environment variable already exists and update it
    while (current != NULL) {
        if (strcmp(current->key, name) == 0) {
            free(current->value);  // Free the old value
            current->value = strdup(value);  // Set new value
            if (!current->value) {
                perror("Memory allocation failed for environment value");
                return -1;
            }
            return 0;  // Successfully updated
        }
        last = current;
        current = current->next;
    }

    // If not found, add new environment variable
    t_env_var *new_var = malloc(sizeof(t_env_var));
    if (!new_var) {
        perror("Memory allocation failed for new environment variable");
        return -1;
    }

    new_var->key = strdup(name);
    if (!new_var->key) {
        free(new_var);
        perror("Memory allocation failed for environment key");
        return -1;
    }
    new_var->value = strdup(value);
    if (!new_var->value) {
        free(new_var->key);
        free(new_var);
        perror("Memory allocation failed for environment value");
        return -1;
    }
    new_var->next = NULL;

    if (last) {
        last->next = new_var;  // Append to the end of the list
    } else {
        *env_head = new_var;  // New head of the list if list was empty
    }

    return 0;
}

char *find_env_var(t_env_var *head, const char *key) {
    while (head != NULL) {
        if (strcmp(head->key, key) == 0) {
            return head->value;
        }
        head = head->next;
    }
    return NULL; // Not found
}

int cd_cmd(t_data *data, t_command *cmd) {
    char *oldpwd = getcwd(NULL, 0);
    if (check_args(cmd) != EXIT_SUCCESS) {
        free(oldpwd);
        data->exit_status = EXIT_FAILURE;
        return EXIT_FAILURE;
    }

    char *target = (cmd->argc > 1) ? cmd->argv->next->value : NULL;
    char *home = find_env_var(data->env_head, "HOME");
    char *oldPath = find_env_var(data->env_head, "OLDPWD");

    target = target ? target : (home ? home : "/");
    if (strcmp(target, "-") == 0) {
        target = oldPath ? oldPath : "/";
        if (!oldPath) {
            fprintf(stderr, "cd: OLDPWD not set\n");
            free(oldpwd);
            data->exit_status = EXIT_FAILURE;
            return EXIT_FAILURE;
        }
        printf("%s\n", oldPath);
    }

    int status = chdir(target);
    if (status != 0) {
        perror("cd");
        free(oldpwd);
        data->exit_status = EXIT_FAILURE;
        return EXIT_FAILURE;
    }

    char *newpwd = getcwd(NULL, 0);
    if (!newpwd) newpwd = strdup("");
    if (set_env_var(&(data->env_head), "OLDPWD", oldpwd) != 0) {
    fprintf(stderr, "cd: error updating OLDPWD environment variable\n");
    free(oldpwd);
    free(newpwd);
    data->exit_status = EXIT_FAILURE;
    return EXIT_FAILURE;
}

if (set_env_var(&(data->env_head), "PWD", newpwd) != 0) {
    fprintf(stderr, "cd: error updating PWD environment variable\n");
    free(oldpwd);
    free(newpwd);
    data->exit_status = EXIT_FAILURE;
    return EXIT_FAILURE;
}


    free(oldpwd);
    free(newpwd);
    data->exit_status = EXIT_SUCCESS;
    return EXIT_SUCCESS;
}



int is_builtin(const char *command)
{
    return (strcmp(command, "cd") == 0 || strcmp(command, "echo") == 0 ||
            strcmp(command, "pwd") == 0 || strcmp(command, "export") == 0 ||
            strcmp(command, "unset") == 0 || strcmp(command, "env") == 0 ||
            strcmp(command, "exit") == 0);
}


void add_or_update_env_var(t_env_var **head, const char *key, const char *value) {
    t_env_var *current = *head;
    t_env_var *prev = NULL;

    // Search for existing variable and update
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            free(current->value);
            current->value = strdup(value);
            return;
        }
        prev = current;
        current = current->next;
    }

    // Create new variable
    t_env_var *new_var = malloc(sizeof(t_env_var));
    if (!new_var) return; // Handle malloc failure
    new_var->key = strdup(key);
    new_var->value = strdup(value);
    new_var->next = NULL;

    if (prev) {
        prev->next = new_var;
    } else {
        *head = new_var;
    }
}

int remove_env_var(t_env_var **head, const char *key) {
    t_env_var *current = *head;
    t_env_var *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                *head = current->next;
            }
            free(current->key);
            free(current->value);
            free(current);
            return 0; // Success
        }
        prev = current;
        current = current->next;
    }
    return -1; // Not found
}

void free_env(t_env_var *env) {
    t_env_var *current = env;
    while (current) {
        t_env_var *next = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next;
    }
}
