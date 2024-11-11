/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 18:32:49 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/11 14:03:18 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Libft/libft.h"
#include "../includes/minishell.h"

t_command	*init_command(char *command)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		perror("Failed to allocate memory for command structure");
		return (NULL);
	}
	ft_memset(cmd, 0, sizeof(t_command));
	if (command)
		cmd->command = ft_strdup(command);
	else
		cmd->command = NULL;
	cmd->argv = NULL;
	cmd->option = NULL;
	cmd->next = NULL;
	cmd->argc = 0;
	cmd->argv_array = NULL;
	printf("Initialized command - Command: '%s'\n", cmd->command);
	return (cmd);
}

int	set_command(t_command *cmd_struct, t_token *current_token)
{
	if (!cmd_struct || !current_token)
		return (-1);
	if (cmd_struct->command)
	{
		free(cmd_struct->command);
		cmd_struct->command = NULL;
	}
	cmd_struct->command = ft_strdup(current_token->value);
	if (!(cmd_struct->command))
		return (-1);
	return (0);
}

int	add_token_to_list(t_token **token_list, t_token *token)
{
	t_token	*new_node;
	t_token	*current;

	if (!token_list || !token)
		return (-1);
	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (-1);
	new_node->type = token->type;
	new_node->value = ft_strdup(token->value);
	new_node->is_quoted = token->is_quoted;
	new_node->next = NULL;
	if (!*token_list)
	{
		*token_list = new_node;
	}
	else
	{
		current = *token_list;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
	return (0);
}

int	add_option_to_command(t_command *cmd, t_token *token)
{
	if (!cmd || !token)
		return (-1);
	return (add_token_to_list(&(cmd->option), token));
}

void	link_command(t_data *data, t_command *new_command)
{
	t_command	*last_command;

	if (!data->commands)
	{
		data->commands = new_command;
	}
	else
	{
		last_command = data->commands;
		while (last_command->next)
			last_command = last_command->next;
		last_command->next = new_command;
	}
}
