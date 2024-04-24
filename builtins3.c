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

void remove_var(t_data *data, const char *var_name)
{
    int len = ft_strlen(var_name);
    char **current = data->env;

    while (*current && ft_strncmp(*current, var_name, len))
    {
        current++;
    }
    if (*current && (*current)[len] == '=')
    {
        free(*current);
        while (*(current + 1))
        {
            *current = *(current + 1);
            current++;
        }
        *current = NULL;
    }
    else
    {
        ft_printf("Variable not found.\n");
    }
}
int valid_identifier(const char *name)
{
    const char *valid_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

    if (!name || *name == '\0' || (!ft_isalpha(*name) && *name != '_'))
        return 0;
    name++;
    if (*name && ft_strspn(name, valid_chars) != ft_strlen(name))
        return 0;
    return 1;
}

int unset_cmd(t_data *data, t_command *cmd)
{
    t_token *arg = cmd->argv;

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
    return 0;
}
