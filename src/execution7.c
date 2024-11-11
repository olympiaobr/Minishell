/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution7.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:58:01 by jasnguye          #+#    #+#             */
/*   Updated: 2024/05/13 12:43:22 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Libft/libft.h"
#include "../includes/minishell.h"

void	check_error(int fd)
{
	if (fd == -1)
	{
		perror("open output file");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2 for output file");
		close(fd);
		exit(EXIT_FAILURE);
	}
}

void	child_process_heredoc(t_data *data, char *path, char **argv,
		int *pipe_fd)
{
	int	fd;
	int	flags;

	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	close(pipe_fd[0]);
	if (data->output_file)
	{
		flags = O_WRONLY | O_CREAT;
		if (data->append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		fd = open(data->output_file, flags, 0644);
		check_error(fd);
		close(fd);
	}
	execve(path, argv, NULL);
	perror("execve");
	exit(EXIT_FAILURE);
}

void	parent_process_heredoc(t_data *data, int *pipe_fd)
{
	ssize_t	bytes_written;

	close(pipe_fd[0]);
	if (data->heredoc_input)
	{
		bytes_written = write(pipe_fd[1], data->heredoc_input,
				strlen(data->heredoc_input));
		if (bytes_written == -1)
		{
			perror("write to pipe");
			exit(EXIT_FAILURE);
		}
		if (data->heredoc_input[strlen(data->heredoc_input) - 1] != '\n')
		{
			write(pipe_fd[1], "\n", 1);
		}
		free(data->heredoc_input);
		data->heredoc_input = NULL;
	}
	close(pipe_fd[1]);
	if (wait(NULL) == -1)
	{
		perror("wait");
		exit(EXIT_FAILURE);
	}
}
