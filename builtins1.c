/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:32:07 by olobresh          #+#    #+#             */
/*   Updated: 2024/04/15 17:32:11 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

int execute_builtin(t_command *cmd, t_data *data)
{
    if (!cmd || !cmd->command)
        return (-1);

    if (ft_strcmp(cmd->command, "cd") == 0)
        return (cd_cmd(cmd, data));
    else if (ft_strcmp(cmd->command, "echo") == 0)
        return (echo_cmd(cmd));
    else if (ft_strcmp(cmd->command, "pwd") == 0)
        return (pwd_cmd());
    else if (ft_strcmp(cmd->command, "export") == 0)
        return (export_cmd(cmd, data));
    else if (ft_strcmp(cmd->command, "unset") == 0)
        return (unset_cmd(cmd, data));
    else if (ft_strcmp(cmd->command, "env") == 0)
        return (env_cmd(data));
    else if (ft_strcmp(cmd->command, "exit") == 0)
        return (exit_cmd(cmd, data));
    return (-1);
}
int pwd_cmd(void)
{
    char *cwd;
    int write_result;

    cwd = getcwd(NULL, 0);
    if (!cwd)
	{
        perror("minishell: pwd");
        return (EXIT_FAILURE);
    }
    write_result = ft_putendl_fd(cwd, STDOUT_FILENO);
    free(cwd);
    if (write_result < 0)
	{
        perror("minishell: write error");
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

int env_cmd(t_data *data)
{
    int i = 0;
    char **envp = data->env;

    if (data->commands && data->commands->argc > 1)
	{
        ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
        return EXIT_FAILURE;
    }
    while (envp[i])
	{
        if (ft_strchr(envp[i], '='))
		{
            if (ft_putendl_fd(envp[i], STDOUT_FILENO) < 0)
			{
                perror("minishell: write error");
                return EXIT_FAILURE;
            }
        }
        i++;
    }
    return EXIT_SUCCESS;
}
