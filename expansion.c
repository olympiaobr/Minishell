/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 17:27:10 by jasnguye          #+#    #+#             */
/*   Updated: 2024/03/29 13:10:18 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

char *get_value(char *variable_name)
{
	return getenv(variable_name);
}
void replace_variable(t_token *token)
{
	char *variable_name = token->value + 1;
	char *value = get_value(variable_name);

	if(!value)
	{
		ft_printf("environment variable doesn't exist\n");
	}
	else
	{
		free(token->value);
		token->value = ft_strdup(value);
	}
}
void expansion(t_data *data) //for now: only works if the $ is at the beginning of the variable(because of the way we split the tokens)
{
	t_token *current = data->token_list;
	int i;
	while(current != NULL)
	{
		i = 0;
		if(current->value[i] == '$')
		{
			replace_variable(current);
		}
		current = current->next;
	}
}