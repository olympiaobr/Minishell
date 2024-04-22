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

int is_builtin(const char *command)
{
    return (ft_strcmp(command, "cd") == 0 || ft_strcmp(command, "echo") == 0 ||
            ft_strcmp(command, "pwd") == 0 || ft_strcmp(command, "export") == 0 ||
            ft_strcmp(command, "unset") == 0 || ft_strcmp(command, "env") == 0 ||
            ft_strcmp(command, "exit") == 0);
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

char **expand_env(char **env, int newsize)
{
    char **new_env;
    int j;

    new_env = (char **)malloc(sizeof(char *) * newsize);
    if (new_env == NULL)
    {
        return NULL;
    }
    j = 0;
    while (env[j] != NULL)
    {
        new_env[j] = env[j];
        j++;
    }
    while (j < newsize)
    {
        new_env[j] = NULL;
        j++;
    }
    return new_env;
}

int set_env_var(t_data *data, const char *name, const char *value)
{
    char *new_val;
    size_t name_len;
    int i = 0;

    if (!name || !value || name[0] == '\0' || name[0] == '=' || ft_strchr(name, '=') != NULL)
    {
        fprintf(stderr, "Invalid environment variable name\n");
        return -1;
    }

    new_val = malloc(ft_strlen(name) + ft_strlen(value) + 2); // +2 for '=' and '\0'
    if (new_val == NULL)
    {
        perror("Memory allocation failed for environment variable");
        return -1;
    }
    sprintf(new_val, "%s=%s", name, value);
    name_len = ft_strlen(name);
    while (data->env[i] != NULL)
    {
        if (ft_strncmp(data->env[i], name, name_len) == 0 && data->env[i][name_len] == '=')
        {
            free(data->env[i]);
            data->env[i] = new_val;
            return 0;
        }
        i++;
    }
    char **new_env = expand_env(data->env, i + 2);
    if (new_env == NULL)
    {
        free(new_val);
        fprintf(stderr, "Failed to expand environment space\n");
        return -1;
    }
    new_env[i] = new_val;
    new_env[i + 1] = NULL;
    free(data->env);
    data->env = new_env;
    return 0;
}

int cd_cmd(t_data *data, t_command *cmd)
{
    char *target;
    char *arg_value;
    char *newpwd;
    char *oldpwd;
    int result;

    if (cmd->argc == 1)
    {
        target = get_env_var(data->env, "HOME");
        if (!target)
        {
            target = "/";
        }
    }
    else if (cmd->argc == 2)
    {
        arg_value = cmd->argv->value;
        if (ft_strcmp(arg_value, "-") == 0)
        {
            target = get_env_var(data->env, "OLDPWD");
            if (!target)
            {
                fprintf(stderr, "cd: OLDPWD not set\n");
                return EXIT_FAILURE;
            }
            printf("%s\n", target);
        }
        else if (ft_strcmp(arg_value, "~") == 0)
        {
            target = get_env_var(data->env, "HOME");
            if (!target)
            {
                target = "/";
            }
        }
        else
        {
            target = arg_value;
        }
    }
    else
    {
        fprintf(stderr, "cd: Incorrect usage. Expected one argument or none, but got %d.\n", cmd->argc - 1);
        return EXIT_FAILURE;
    }
    if (chdir(target) != 0)
    {
        perror("cd failed");
        return EXIT_FAILURE;
    }
    newpwd = getcwd(NULL, 0);
    if (!newpwd)
    {
        perror("getcwd failed");
        return EXIT_FAILURE;
    }
    oldpwd = get_env_var(data->env, "PWD");
    result = set_env_var(data, "OLDPWD", oldpwd);
    if (result != 0)
    {
        fprintf(stderr, "cd: error updating OLDPWD environment variable\n");
        free(newpwd);
        return EXIT_FAILURE;
    }
    result = set_env_var(data, "PWD", newpwd);
    if (result != 0)
    {
        fprintf(stderr, "cd: error updating PWD environment variable\n");
        free(newpwd);
        return EXIT_FAILURE;
    }
    free(newpwd);
    return EXIT_SUCCESS;
}
int validate_num(const char *str)
{
    while (*str)
    {
        if (!ft_isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}

int exit_cmd(t_data *data, t_command *cmd)
{
    (void)data;
    int exit_status = 0;

    if (cmd->argc > 2)
    {
        fprintf(stderr, "exit: too many arguments\n");
        return 1;
    }
    if (cmd->argc == 2)
    {
        char *arg = cmd->argv->value;
        if (validate_num(arg))
        {
            exit_status = ft_atoi(arg);
        } else
        {
            fprintf(stderr, "exit: numeric argument required\n");
            exit_status = 255;
        }
    }
    exit(exit_status);
    return (0);
}

