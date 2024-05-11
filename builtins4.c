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

const char	*parse_cd_target(t_data *data, t_command *cmd)
{
	const char	*target;

	if (cmd->argc == 1 || (cmd->argc == 2 && ft_strcmp(cmd->argv->value,
				";") == 0))
	{
		target = get_env_var(data->env, "HOME");
		if (!target)
		{
			printf("cd: HOME not set\n");
			return (NULL);
		}
		return (target);
	}
	return (handle_cd_arguments(data, cmd));
}

int	update_cd_env(t_data *data, const char *oldpwd)
{
	char	*newpwd;
	int		result;

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

int	cd_cmd(t_data *data, t_command *cmd)
{
	const char	*target;
	const char	*oldpwd;

	target = parse_cd_target(data, cmd);
	if (!target)
		return (EXIT_FAILURE);
	if (chdir(target) != 0)
	{
		printf("cd: %s: No such file or directory\n", target);
		return (EXIT_FAILURE);
	}
	oldpwd = get_env_var(data->env, "PWD");
	return (update_cd_env(data, oldpwd));
}

int	validate_num(const char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

int	exit_cmd(t_data *data, t_command *cmd)
{
	int		exit_status;
	char	*arg;

	(void)data;
	exit_status = 0;
	if (cmd->argc > 1)
	{
		arg = cmd->argv->value;
		if (validate_num(arg))
		{
			exit_status = ft_atoi(arg);
			exit(exit_status);
		}
		else
		{
			printf("exit\nbash: exit: %s: numeric argument required\n", arg);
			exit_status = 255;
			exit(exit_status);
		}
	}
	exit(exit_status);
}
