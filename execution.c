/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 11:14:25 by jasnguye          #+#    #+#             */
/*   Updated: 2024/05/13 10:59:09 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

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
	while ((ret = waitpid(pid, &status, 0)) == -1)
	{
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

void	execute_external_command(t_data *data, t_command *cmd)
{
	int		io[2];
	char	**argv;
	pid_t	pid;

	if (!validate_command(cmd))
		return ;
	determine_io_channels(data, cmd->command_index, io);
	if (!validate_io_channels(io))
		return ;
	argv = create_argv(cmd);
	if (!argv)
		return ;
	pid = fork();
	if (pid == -1)
	{
		perror("Fork failed");
		free(argv);
		return ;
	}
	if (pid == 0)
	{
		setup_io_channels(io);
		execute_command(cmd, argv, data->env);
	}
	else
	{
		handle_parent_process(data, pid);
		free(argv);
	}
}

void	execute_simple_command(t_data *data, t_command *cmd)
{
	int	exit_status;

	if (check_builtin(cmd->command))
	{
		exit_status = execute_builtin(cmd, data);
		data->exit_status = exit_status;
		return ;
	}
	if (data->heredoc == 1)
		handle_heredocs(data, cmd);
	else if (ft_strcmp(data->token_list->value, "expr") == 0
		&& data->token_list->next
		&& ft_strcmp(data->token_list->next->next->value, "+") == 0)
		handle_expr_function(data);
	else
		execute_external_command(data, cmd);
	if (data->std_input_fd != STDIN_FILENO)
	{
		close(data->std_input_fd);
		data->std_input_fd = STDIN_FILENO;
	}
	if (data->std_output_fd != STDOUT_FILENO)
	{
		close(data->std_output_fd);
		data->std_output_fd = STDOUT_FILENO;
	}
}

void	execution(t_data *data)
{
	if (!data || !data->commands || !data->commands->command)
		return ;
	if (check_valid_command(data) != 1)
	{
		ft_printf("%s: command not found\n", data->commands->command);
		data->exit_status = 127;
		return ;
	}
	count_commands(data);
	if (operators_setup(data) != 0)
	{
		ft_printf("Failed to setup redirections.\n");
		return ;
	}
	if (data->count_cmd > 1)
	{
		ft_printf("Debug: Executing pipeline...\n");
		create_pipes(data);
		execute_pipeline(data, data->commands);
	}
	else
		execute_simple_command(data, data->commands);
	free_commands(data->commands);
	data->commands = NULL;
	free_pipesfd(data);
}
