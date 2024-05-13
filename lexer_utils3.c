/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:40:33 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/13 12:57:24 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	free_token(t_token *token)
{
	if (token != NULL)
	{
		free(token->value);
		free(token);
	}
}

void	free_tokens(t_data *data)
{
	t_token	*current;
	t_token	*next;

	if (data == NULL)
		return ;
	current = data->token_list;
	while (current != NULL)
	{
		next = current->next;
		free_token(current);
		current = next;
	}
	data->token_list = NULL;
	if (data->user_input != NULL)
	{
		free(data->user_input);
		data->user_input = NULL;
	}
}

void	free_token_chain(t_token *token)
{
	t_token	*next_token;

	while (token)
	{
		next_token = token->next;
		free(token->value);
		free(token);
		token = next_token;
	}
}

char	*trim_value(const char *val)
{
	return (ft_strtrim(val, " \t\n\v\f\r"));
}

t_token_type	determine_type(char *operator)
{
	if (ft_strncmp(operator, "|", 1) == 0)
		return (T_PIPE);
	if (ft_strncmp(operator, "<<", 2) == 0)
		return (T_HEREDOC);
	if (ft_strncmp(operator, ">>", 2) == 0)
		return (T_APPEND);
	if (ft_strncmp(operator, "<", 1) == 0)
		return (T_IN);
	if (ft_strncmp(operator, ">", 1) == 0)
		return (T_OUT);
	return (T_COMMAND);
}
