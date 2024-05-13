/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/04 18:30:22 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/13 11:08:47 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

char	*resolve_command_path(char *command, t_data *data)
{
	char	*path_env;
	char	*resolved_path;
	char	*path;

	if (command[0] == '/' || (command[0] == '.' && command[1] == '/'))
		return (ft_strdup(command));
	path_env = cust_getenv("PATH", data);
	if (!path_env)
		return (NULL);
	path = ft_strdup(path_env);
	if (!path)
		return (NULL);
	resolved_path = get_cmd_fullpath(command, path);
	free(path);
	return (resolved_path);
}

void	setup_and_execute_command(t_data *data, t_command *cmd, int j, int *io)
{
	char	**args;
	char	*cmd_path;

	if (j != 0)
	{
		dup2(io[0], STDIN_FILENO);
		close(io[0]);
	}
	if (cmd->next != NULL)
	{
		dup2(io[1], STDOUT_FILENO);
		close(io[1]);
	}
	args = prepare_command_args(cmd);
	cmd_path = resolve_command_path(args[0], data);
	execve(cmd_path, args, data->env);
	perror("execve");
	exit(EXIT_FAILURE);
}

void	cmd_process(t_data *data, t_command *cmd, int *j, int io[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		determine_io_channels(data, *j, io);
		setup_and_execute_command(data, cmd, *j, io);
	}
}

void	close_unused_pipes(t_data *data, int j)
{
	if (j != 0)
	{
		close(data->pipesfd[j - 1][0]);
	}
	close(data->pipesfd[j][1]);
}

void	execute_pipeline(t_data *data, t_command *cmd)
{
	int	j;
	int	i;
	int	io[2];

	j = 0;
	i = 0;
	while (cmd != NULL && i < data->count_cmd)
	{
		cmd_process(data, cmd, &j, io);
		if (j < data->count_cmd - 1 && cmd->next != NULL)
		{
			close_unused_pipes(data, j);
			j++;
		}
		cmd = cmd->next;
		i++;
	}
	wait_and_close_pipes(data, data->count_cmd);
}
