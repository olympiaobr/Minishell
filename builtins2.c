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

int check_arguments(t_command *cmd)
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
int set_env_var(char **envp, const char *name, const char *value)
{
    char *new_val;
    int i;
    size_t name_len;

    if (!name || !value || name[0] == '\0' || name[0] == '=' || ft_strchr(name, '=') != NULL)
    {
        fprintf(stderr, "Invalid environment variable name\n");
        return -1;  // Invalid name
    }
    new_val = malloc(ft_strlen(name) + ft_strlen(value) + 2);
    if (new_val == NULL)
    {
        perror("Memory allocation failed for environment variable");
        return -1;
    }
    sprintf(new_val, "%s=%s", name, value);

    // Replace or add new variable
    name_len = strlen(name);
    i = 0;
    while (envp[i] != NULL)
    {
        if (ft_strncmp(envp[i], name, name_len) == 0 && envp[i][name_len] == '=')
        {
            free(envp[i]);
            envp[i] = new_val;
            return 0;
        }
        i++;
    }

     // Handle adding a new variable when no match is found
    i = 0;
    while (envp[i] != NULL)
    {
        i++;
    }
    envp[i] = new_val;
    envp[i + 1] = NULL;
    return 0;
}

int cd_cmd(t_data *data, t_command *cmd)
{
    char *oldpwd;
    char *target;
    char *newpwd;
    char *home;
    char *oldPath;
    int status;

    oldpwd = getcwd(NULL, 0);
    if (check_args(cmd) != EXIT_SUCCESS)
    {
        free(oldpwd);
        data->exit_status = EXIT_FAILURE;
        return EXIT_FAILURE;
    }

    if (cmd->argc > 1)
    {
        target = cmd->argv->next->value;
    }
    else
    {
        target = NULL;
    }
    if (target == NULL || strcmp(target, "~") == 0)
    {
        home = get_env_var(data->env, "HOME");
        if (home != NULL)
        {
            target = home;
        }
        else
        {
            target = "/";
        }
    }
    else if (ft_strcmp(target, "-") == 0)
    {
        oldPath = get_env_var(data->env, "OLDPWD");
        if (oldPath == NULL)
        {
            fprintf(stderr, "cd: OLDPWD not set\n");
            free(oldpwd);
            data->exit_status = EXIT_FAILURE;
            return EXIT_FAILURE;
        }
        printf("%s\n", oldPath);
        target = oldPath;
    }

    status = chdir(target);
    if (status != 0)
    {
        perror("cd");
        free(oldpwd);
        data->exit_status = EXIT_FAILURE;
        return EXIT_FAILURE;
    }
    newpwd = getcwd(NULL, 0);
    if (newpwd == NULL)
        newpwd = ft_strdup("");

    if (set_env_var(data->env, "OLDPWD", oldpwd) != 0)
    {
        fprintf(stderr, "cd: error updating OLDPWD environment variable\n");
        free(oldpwd);
        free(newpwd);
        data->exit_status = EXIT_FAILURE;
        return EXIT_FAILURE;
    }
    if (set_env_var(data->env, "PWD", newpwd) != 0)
    {
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

