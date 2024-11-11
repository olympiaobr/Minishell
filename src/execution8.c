/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution8.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:02:49 by jasnguye          #+#    #+#             */
/*   Updated: 2024/05/13 11:03:32 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Libft/libft.h"
#include "../includes/minishell.h"

void	open_input_file(t_data *data)
{
	data->std_input_fd = open(data->input_file, O_RDONLY);
	if (data->std_input_fd == -1)
	{
		perror("Failed to open input file");
		data->std_input_fd = STDIN_FILENO;
	}
}

int	operators_setup(t_data *data)
{
	int	flags;

	if (data->input_file)
	{
		open_input_file(data);
	}
	else
		data->std_input_fd = STDIN_FILENO;
	if (data->output_file)
	{
		flags = O_WRONLY | O_CREAT;
		if (data->append)
			flags |= O_APPEND;
		data->std_output_fd = open(data->output_file, flags, 0644);
		if (data->std_output_fd == -1)
		{
			perror("Failed to open output file");
			data->std_output_fd = STDOUT_FILENO;
		}
	}
	else
		data->std_output_fd = STDOUT_FILENO;
	return (0);
}

void	set_input_channel(t_data *data, int cmd_index, int default_input,
		int io[2])
{
	if (cmd_index == 0)
	{
		if (data->std_input_fd > 0)
			io[0] = data->std_input_fd;
		else
			io[0] = default_input;
	}
	else if (data->pipesfd && cmd_index > 0)
		io[0] = data->pipesfd[cmd_index - 1][0];
}
