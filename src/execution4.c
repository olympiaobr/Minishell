/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 15:24:13 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/13 13:22:12 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Libft/libft.h"
#include "../includes/minishell.h"

void	display_all_env_vars(char **env)
{
	int	i;

	i = 0;
	while (env[i] != NULL)
	{
		printf("declare -x %s\n", env[i]);
		i++;
	}
}

int	export_cmd(t_data *data, t_command *cmd)
{
	if (!cmd->argv)
	{
		display_all_env_vars(data->env);
		return (0);
	}
	handle_export_operation(data, cmd->argv);
	return (0);
}

int	check_builtin(const char *command)
{
	return (ft_strcmp(command, "cd") == 0 || ft_strcmp(command, "echo") == 0
		|| ft_strcmp(command, "pwd") == 0 || ft_strcmp(command, "export") == 0
		|| ft_strcmp(command, "unset") == 0 || ft_strcmp(command, "env") == 0
		|| ft_strcmp(command, "exit") == 0);
}

void	execute_command(t_command *cmd, char **argv, char **env)
{
	execve(cmd->path, argv, env);
	perror("Execve failed");
	exit(EXIT_FAILURE);
}

void	handle_parent_process(t_data *data, pid_t pid)
{
	int	status;
	int	ret;

	status = 0;
	ret = waitpid(pid, &status, 0);
	while (ret == -1)
	{
		ret = waitpid(pid, &status, 0);
	}
	if (ret == -1)
	{
		perror("waitpid failed");
	}
	else if (WIFEXITED(status))
	{
		data->exit_status = WEXITSTATUS(status);
	}
}
