/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 15:22:21 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/06 16:01:11 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

void	free_command(t_command *cmd)
{
	int	index;

	if (!cmd)
		return ;
	free(cmd->path);
	free(cmd->command);
	free_token_chain(cmd->argv);
	free_token_chain(cmd->option);
	if (cmd->argv_array)
	{
		index = 0;
		while (index < cmd->argc)
		{
			free(cmd->argv_array[index]);
			index++;
		}
		free(cmd->argv_array);
	}
	free(cmd);
}

void	free_commands(t_command *cmd)
{
	t_command	*next_cmd;

	while (cmd)
	{
		next_cmd = cmd->next;
		free_command(cmd);
		cmd = next_cmd;
	}
}

void	free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i] != NULL)
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
}

void	free_data_resources(t_data *data)
{
	free(data->input_file);
	data->input_file = NULL;
	free(data->output_file);
	data->output_file = NULL;
	free_tokens(data);
	free_commands(data->commands);
}

void	free_all(t_data *data)
{
	if (!data)
		return ;
	free_array(data->env);
	data->env = NULL;
	free_array(data->path_dirs);
	data->path_dirs = NULL;
	free_data_resources(data);
}
