/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 11:54:01 by olobresh          #+#    #+#             */
/*   Updated: 2024/04/24 11:54:03 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

int update_existing_env(t_data *data, char *new_val, int i)
{
    free(data->env[i]);
    data->env[i] = new_val;
    return (0);
}

int update_or_add_env(t_data *data, char *new_val, const char *name)
{
    size_t name_len;
    int i;

    name_len = ft_strlen(name);
    i = 0;
    while (data->env[i] != NULL)
    {
        if (ft_strncmp(data->env[i], name, name_len) == 0 && data->env[i][name_len] == '=')
        {
            return (update_existing_env(data, new_val, i));
        }
        i++;
    }
    return (add_new_env(data, new_val, i));
}

int set_env_var(t_data *data, const char *name, const char *value)
{
    char *new_val;
    size_t name_len;
    size_t value_len;

    if (!name || !value || name[0] == '\0' || name[0] == '=' || strchr(name, '=') != NULL)
    {
        fprintf(stderr, "Invalid environment variable name\n");
        return -1;
    }
    name_len = ft_strlen(name);
    value_len = ft_strlen(value);
    new_val = malloc(name_len + value_len + 2);
    if (new_val == NULL)
    {
        perror("Memory allocation failed for environment variable");
        return -1;
    }
    ft_strncpy(new_val, name, name_len);
    new_val[name_len] = '=';
    ft_strncpy(new_val + name_len + 1, value, value_len);
    new_val[name_len + value_len + 1] = '\0';
    return (update_or_add_env(data, new_val, name));
}
const char *determine_target(t_data *data, const char *arg_value)
{
    const char *target;

    if (ft_strcmp(arg_value, "-") == 0)
    {
        target = get_env_var(data->env, "OLDPWD");
        if (!target)
        {
            printf("cd: OLDPWD not set\n");
            return NULL;
        }
        printf("%s\n", target);
        return target;
    }
    if (ft_strcmp(arg_value, "~") == 0)
    {
        target = get_env_var(data->env, "HOME");
        if (!target)
        {
            printf("cd: HOME not set\n");
            return NULL;  // Remove the fallback to "/" to adhere to typical Unix behavior
        }
        return target;
    }
    return arg_value;
}

const char *handle_cd_arguments(t_data *data, t_command *cmd)
{
    const char *arg_value;

    if (cmd->argc == 2 && ft_strcmp(cmd->argv->value, ";") != 0)
    {
        arg_value = cmd->argv->value;
        return determine_target(data, arg_value);
    }
    else if (cmd->argc == 2 && ft_strcmp(cmd->argv->value, ";") == 0)
    {
        printf("cd: Unexpected ';'\n");
        return NULL;
    }
    else
    {
        printf("cd: Incorrect usage. Expected one argument or none, but got %d.\n", cmd->argc - 1);
        return NULL;
    }
}


