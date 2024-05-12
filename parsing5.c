/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 13:01:01 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/12 13:01:03 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

int	apply_redirection(t_data *data, t_token *token)
{
	char	*file_name;
	int		result;

	file_name = NULL;
	if (data->output_file_present == 1)
	{
		file_name = ft_strdup(data->output_file);
		printf("here output file present\n");
	}
	else if (data->heredoc == 1 && token->type == T_HEREDOC)
	{
		file_name = ft_strdup("heredoc_tempfile");
	}
	else
	{
		file_name = ft_strdup(token->next->value);
	}
	result = setup_redirection(data, token, file_name);
	free(file_name);
	return (result);
}

int	process_command_and_args(t_data *data, t_token *current_token,
		t_command **current_cmd, int *new_cmd)
{
	int	process_result;

	process_result = process_commands(data, current_token, current_cmd);
	if (process_result != 0)
	{
		ft_error("Error: Failed to process command or argument.\n");
		return (1);
	}
	if (current_token->type == T_COMMAND)
	{
		*new_cmd = 0;
	}
	return (0);
}

int	handle_redir(t_data *data, t_token *current_token)
{
	if (current_token->type == T_IN || current_token->type == T_OUT
		|| current_token->type == T_APPEND || current_token->type == T_HEREDOC)
	{
		return (apply_redirection(data, current_token));
	}
	return (0);
}

int	handle_token(t_data *data, t_token *current_token, t_command **current_cmd,
		int *new_cmd)
{
	if (!data || !current_token || !current_cmd || !new_cmd)
		return (-1);
	if (*new_cmd || current_token->type == T_COMMAND
		|| current_token->type == T_ARGUMENT)
	{
		if (process_command_and_args(data, current_token, current_cmd,
				new_cmd) != 0)
			return (1);
	}
	else
	{
		if (handle_redir(data, current_token) != 0)
			return (1);
	}
	return (0);
}

int	parser(t_data *data)
{
	t_command	*current_cmd;
	t_token		*current_token;
	int			new_cmd;

	if (!data || !data->token_list)
		return (-1);
	current_cmd = NULL;
	current_token = data->token_list;
	new_cmd = 1;
	while (current_token != NULL)
	{
		if (current_token->type == T_PIPE)
		{
			new_cmd = 1;
		}
		else
		{
			if (handle_token(data, current_token, &current_cmd, &new_cmd) != 0)
				return (1);
		}
		current_token = current_token->next;
	}
	return (0);
}
