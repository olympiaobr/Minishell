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

int update_cd_env(t_data *data, const char *oldpwd)
{
    char *newpwd;
    int result;

    newpwd = getcwd(NULL, 0);
    if (!newpwd)
    {
        perror("getcwd failed");
        return (EXIT_FAILURE);
    }
    result = set_env_var(data, "OLDPWD", oldpwd);
    if (result != 0)
    {
        printf("cd: error updating OLDPWD environment variable\n");
        free(newpwd);
        return (EXIT_FAILURE);
    }
    result = set_env_var(data, "PWD", newpwd);
    free(newpwd);
    if (result != 0)
    {
        printf("cd: error updating PWD environment variable\n");
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

int cd_cmd(t_data *data, t_command *cmd)
{
    const char *target;
    const char *oldpwd;
    int result;

    target = parse_cd_target(data, cmd);
    if (!target)
        return (EXIT_FAILURE);
    if (chdir(target) != 0)
    {
        perror("cd failed");
        return EXIT_FAILURE;
    }
    oldpwd = get_env_var(data->env, "PWD");
    result = update_cd_env(data, oldpwd);
    return (result);
}

int validate_num(const char *str)
{
    while (*str)
    {
        if (!ft_isdigit(*str))
            return 0;
        str++;
    }
    return (1);
}

int exit_cmd(t_data *data, t_command *cmd)
{
    (void)data;
    int exit_status = 0;

    if (cmd->argc > 2)
    {
        fprintf(stderr, "exit: too many arguments\n");
        return (1);
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
    int len;
    char **current;

    len = ft_strlen(var_name);
    current = data->env;
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
