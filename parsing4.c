/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 12:40:04 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/12 15:00:02 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

void	clear_previous_redirections(t_data *data, t_token *token)
{
	if (token->type == T_OUT || (token->type == T_APPEND && !data->append))
	{
		if (data->output_file && data->std_output_fd != STDOUT_FILENO)
		{
			close(data->std_output_fd);
			free(data->output_file);
			data->output_file = NULL;
			data->std_output_fd = STDOUT_FILENO;
		}
	}
	else if (token->type == T_IN || token->type == T_HEREDOC)
	{
		if (data->input_file && data->std_input_fd != STDIN_FILENO)
		{
			close(data->std_input_fd);
			free(data->input_file);
			data->input_file = NULL;
			data->std_input_fd = STDIN_FILENO;
		}
	}
}

void	setup_append_mode(t_data *data, int fd, token_type type)
{
	if (type == T_APPEND)
		data->append = 1;
	else
		data->append = 0;
	if (data->std_output_fd != STDOUT_FILENO)
	{
		close(data->std_output_fd);
	}
	if (type == T_APPEND || type == T_OUT)
	{
		data->std_output_fd = fd;
	}
	else
	{
		data->std_output_fd = STDOUT_FILENO;
	}
}

int	determine_flags(token_type type)
{
	if (type == T_OUT)
	{
		return (O_WRONLY | O_CREAT | O_TRUNC);
	}
	else if (type == T_APPEND)
	{
		return (O_WRONLY | O_CREAT | O_APPEND);
	}
	else if (type == T_IN || type == T_HEREDOC)
	{
		return (O_RDONLY);
	}
	return (0);
}

int	open_redir_file(char *filename, int flags)
{
	char	*file_path;
	int		fd;

	file_path = ft_strdup(filename);
	printf("file name is: %s\n", filename);
	if (!file_path)
	{
		perror("Failed to allocate memory for file path");
		return (-1);
	}
	fd = open(file_path, flags, 0644);
	if (fd < 0)
	{
		free(file_path);
		//perror("Failed to open file");
		return (-1);
	}
	free(file_path);
	return (fd);
}

int	setup_redirection(t_data *data, t_token *token, char *filename)
{
	int	fd;
	int	flags;

	flags = determine_flags(token->type);
	fd = open_redir_file(filename, flags);
	if (fd < 0)
	{
		return (-1);
	}
	clear_previous_redirections(data, token);
	if (token->type == T_IN || token->type == T_HEREDOC)
	{
		data->input_file = ft_strdup(filename);
		data->std_input_fd = fd;
	}
	else
	{
		setup_append_mode(data, fd, token->type);
		data->output_file = ft_strdup(filename);
	}
	return (0);
}
