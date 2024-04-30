/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 11:57:12 by olobresh          #+#    #+#             */
/*   Updated: 2024/04/24 11:57:13 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

int export_cmd(t_data *data, t_command *cmd)
{
    t_token *arg = cmd->argv;
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
            char *existing_value = get_env_var(data->env, var_name);
            if (existing_value)
            {
                if (value)
                {
                    set_env_var(data, var_name, value);
                }
                else
                {
                    set_env_var(data, var_name, "");
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
