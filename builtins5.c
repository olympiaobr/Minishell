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

void process_var_update(t_data *data, char *var_name, char *value)
{
    char *existing_value;

	existing_value = get_env_var(data->env, var_name);
    if (existing_value)
    {
        if (value)
            set_env_var(data, var_name, value);
        else
            set_env_var(data, var_name, existing_value);
    }
    else
    {
        if (value)
        {
            if (set_env_var(data, var_name, value) == -1)
                fprintf(stderr, "Failed to export variable: %s\n", var_name);
        }
        else
        {
            if (set_env_var(data, var_name, "") == -1)
                fprintf(stderr, "Failed to export variable: %s\n", var_name);
        }
    }
}

void handle_export_operation(t_data *data, t_token *arg)
{
    char *var_name;
    char *value;

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
            process_var_update(data, var_name, value);
        }
        arg = arg->next;
    }
}

void display_all_env_vars(char **env)
{
    int i;

	i = 0;
    while (env[i] != NULL)
    {
        printf("declare -x %s\n", env[i]);
        i++;
    }
}
