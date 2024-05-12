/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 12:11:55 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/12 12:11:57 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

t_command	*create_command(t_data *data, t_token *token)
{
	t_command	*new_command;

	new_command = (t_command *)malloc(sizeof(t_command));
	if (!new_command)
	{
		perror("Failed to allocate memory for new command node");
		return (NULL);
	}
	ft_memset(new_command, 0, sizeof(t_command));
	new_command->command = ft_strdup(token->value);
	new_command->argv = NULL;
	new_command->argc = 1;
	new_command->next = NULL;
	link_command(data, new_command);
	return (new_command);
}

static void	append_arg_to_command(t_command *last_command, t_token *new_arg)
{
	t_token	*current;

	if (!last_command->argv)
	{
		last_command->argv = new_arg;
	}
	else
	{
		current = last_command->argv;
		while (current->next)
			current = current->next;
		current->next = new_arg;
	}
	last_command->argc++;
}

int	link_arg_to_command(t_command *last_command, t_token *token)
{
	t_token	*new_arg;

	if (!last_command || !token || token->type != T_ARGUMENT)
	{
		return (-1);
	}
	new_arg = (t_token *)malloc(sizeof(t_token));
	if (!new_arg)
	{
		perror("Failed to allocate memory for new argument");
		return (-1);
	}
	new_arg->type = token->type;
	new_arg->value = ft_strdup(token->value);
	if (!new_arg->value)
	{
		free(new_arg);
		return (-1);
	}
	new_arg->next = NULL;
	new_arg->is_quoted = token->is_quoted;
	append_arg_to_command(last_command, new_arg);
	return (0);
}

int	handle_command_creation(t_data *data, t_token *token,
		t_command **current_cmd)
{
	if (token->type != T_PIPE)
	{
		*current_cmd = create_command(data, token);
		if (!(*current_cmd))
			return (-1);
		if (set_command(*current_cmd, token) != 0)
			return (-1);
	}
	data->create_new_command = 0;
	return (0);
}

int	process_commands(t_data *data, t_token *token, t_command **current_cmd)
{
	if (!data || !token || !current_cmd)
		return (-1);
	if (data->create_new_command || token->type == T_COMMAND)
	{
		return (handle_command_creation(data, token, current_cmd));
	}
	else if (token->type == T_ARGUMENT)
	{
		if (token->value[0] == '-')
		{
			return (add_option_to_command(*current_cmd, token));
		}
		else
		{
			return (link_arg_to_command(*current_cmd, token));
		}
	}
	return (0);
}
