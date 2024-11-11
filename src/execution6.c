/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution6.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 12:12:13 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/13 12:26:14 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Libft/libft.h"
#include "../includes/minishell.h"

void	handle_expr_function(t_data *data)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		child_process_expr(data);
	}
	else
		parent_process_expr(data, pid);
}

void	child_process_expr(t_data *data)
{
	char	*argv[5];
	char	*exit_status_str_1;
	char	*exit_status_str_2;

	exit_status_str_1 = ft_itoa(data->exit_status);
	exit_status_str_2 = ft_itoa(data->exit_status);
	if (exit_status_str_1 == NULL || exit_status_str_2 == NULL)
	{
		perror("Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	argv[0] = "/usr/bin/expr";
	argv[1] = exit_status_str_1;
	argv[2] = "+";
	argv[3] = exit_status_str_2;
	argv[4] = NULL;
	execve("/usr/bin/expr", argv, NULL);
	perror("execve");
	exit(EXIT_FAILURE);
	free(exit_status_str_1);
	free(exit_status_str_2);
}

void	parent_process_expr(t_data *data, pid_t pid)
{
	int	status;
	int	exit_status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		exit_status = WEXITSTATUS(status);
		data->exit_status = exit_status;
	}
}

void	handle_heredocs(t_data *data, t_command *cmd)
{
	char	**argv;
	int		pipe_fd[2];
	char	*path;

	path = cmd->path;
	argv = malloc(2 * sizeof(char *));
	if (argv == NULL)
	{
		perror("Failed to allocate memory for argv");
		exit(EXIT_FAILURE);
	}
	argv[0] = path;
	argv[1] = NULL;
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		free(argv);
		exit(EXIT_FAILURE);
	}
	execute_heredoc(data, argv, pipe_fd, path);
}

void	execute_heredoc(t_data *data, char **argv, int *pipe_fd, char *path)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(argv);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		child_process_heredoc(data, path, argv, pipe_fd);
	}
	else
	{
		parent_process_heredoc(data, pipe_fd);
	}
	free(argv);
}
