/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 14:35:53 by jasnguye          #+#    #+#             */
/*   Updated: 2024/04/24 12:19:44 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Libft/libft.h"
#include "../includes/minishell.h"

int	find_command(t_token *current, char *dir, t_data *data)
{
	char	full_path[1024];

	if (find_command_path(current->value, dir, full_path))
	{
		if (data->commands->path)
			free(data->commands->path);
		data->commands->path = ft_strdup(full_path);
		return (1);
	}
	return (0);
}

int	process_command(t_token *current, char *cwd, char *dir, t_data *data)
{
	int	command_result;

	command_result = check_builtin_command(current);
	if (command_result == 0)
		command_result = check_command_path(current, cwd, data);
	if (command_result == 0)
		command_result = find_command(current, dir, data);
	return (command_result);
}

int	process_cmd(t_data *data, char *cwd, char *path_copy)
{
	t_token	*current;
	char	*dir;
	int		command_result;

	current = data->token_list;
	dir = custom_strtok(path_copy, ":");
	while (current)
	{
		if (current->type == T_COMMAND)
		{
			command_result = process_command(current, cwd, dir, data);
			if (command_result)
			{
				return (1);
			}
		}
		current = current->next;
	}
	return (0);
}

int	check_valid_command(t_data *data)
{
	char	cwd[1024];
	char	*path_copy;
	int		result;

	path_copy = init_path_vars(data, cwd);
	if (path_copy == NULL)
		return (-1);
	result = process_cmd(data, cwd, path_copy);
	free(path_copy);
	if (result)
	{
		return (1);
	}
	else
	{
		return (-1);
	}
}
