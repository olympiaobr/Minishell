/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 13:50:04 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/10 13:50:06 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

int valid_identifier(const char *name)
{
    const char *valid_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

    if (!name || *name == '\0' || (!ft_isalpha(*name) && *name != '_'))
        return 0;
    name++;
    if (*name && ft_strspn(name, valid_chars) != ft_strlen(name))
        return (0);
    return (1);
}

int unset_cmd(t_data *data, t_command *cmd)
{
    t_token *arg;

	arg = cmd->argv;
    while (arg)
    {
        if (!valid_identifier(arg->value))
        {
            fprintf(stderr, "unset: '%s' is not a valid identifier\n", arg->value);
        }
        else
        {
            ft_printf("Unsetting variable: %s\n", arg->value);
            remove_var(data, arg->value);
        }
        arg = arg->next;
    }
    return (0);
}

int export_cmd(t_data *data, t_command *cmd)
{
    t_token *arg = cmd->argv;
    char *var_name;
    char *value;

    if (!arg)
    {
        int i = 0;
        while (data->env[i] != NULL)
        {
            printf("declare -x %s\n", data->env[i]);
            i++;
        }
        return 0;
    }
    while (arg)
    {
        var_name = custom_strtok(arg->value, "=");
        value = custom_strtok(NULL, "");
        if (!valid_identifier(var_name))
        {
            fprintf(stderr, "export: '%s': not a valid identifier\n", var_name);
        }
        else
        {
            char *existing_value = get_env_var(data->env, var_name);
            if (existing_value)
            {
                if (value)
                {
                    set_env_var(data, var_name, value);
                }
                else
                {
                    set_env_var(data, var_name, existing_value); // Keep existing value if no new value provided
                }
            }
            else
            {
                if (value)
                {
                    if (set_env_var(data, var_name, value) == -1)
                    {
                        fprintf(stderr, "Failed to export variable: %s\n", var_name);
                    }
                }
                else
                {
                    if (set_env_var(data, var_name, "") == -1)
                    {
                        fprintf(stderr, "Failed to export variable: %s\n", var_name);
                    }
                }
            }
        }
        arg = arg->next;
    }
    return 0;
}


int check_builtin(const char *command)
{
    return (ft_strcmp(command, "cd") == 0 || ft_strcmp(command, "echo") == 0 ||
            ft_strcmp(command, "pwd") == 0 || ft_strcmp(command, "export") == 0 ||
            ft_strcmp(command, "unset") == 0 || ft_strcmp(command, "env") == 0 ||
            ft_strcmp(command, "exit") == 0);
}
