/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:40:33 by olobresh          #+#    #+#             */
/*   Updated: 2024/03/27 11:40:35 by olobresh         ###   ########.fr       */
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
