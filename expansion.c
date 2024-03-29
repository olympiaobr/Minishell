/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 17:27:10 by jasnguye          #+#    #+#             */
/*   Updated: 2024/03/29 17:43:57 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

char *get_value(char *variable_name)
{
	return getenv(variable_name);
}
void replace_variable(int start, int end, char *delimiter, t_token *token)
{
	char *variable_name = ft_substr(token->value, start, end - start);
	char *value = get_value(variable_name);
	if(!value)
	{
		ft_printf("\n");// if it does not exist we print a newline like in bash
	}
	else
	{
		free(token->value);
		char *expanded_value = ft_strjoin(value, delimiter);
		token->value = ft_strdup(expanded_value);
	}
}
int	expansion_delimiters(char c)
{
	return (c == ':' || c == '-' || c == '?' || c == '+' || c == '%'
		|| c == '=' || c == '/');
}


void expansion(t_data *data)
{
	t_token *current = data->token_list;
	while (current != NULL)
	{
		int i = 0;
		while (current->value[i] != '\0')
		{
			if (current->value[i] == '$')
			{
				int start = i + 1;
				while (current->value[i] != '\0' && !expansion_delimiters(current->value[i]))
				{
					i++;
				}
				int end = i;
				while (current->value[i] != '\0' && current->value[i] != '$')
				{
					i++;
				}
				int delimiter_index = i;
				replace_variable(start, end, ft_substr(current->value, end, delimiter_index - end), current);
			}
			i++;
			printf("i:%d\n", i);
		}
		current = current->next;
	}
}


/* void expansion(t_data *data) //for now: only works if the $ is at the beginning of the variable(because of the way we split the tokens)
{
	t_token *current = data->token_list;
	int i;
	int end;

	end = 0;
	i = 0;
	while(current != NULL)
	{
		
		while(current->value[i] != '\0' && !expansion_delimiters(current->value[i]))
		{
			if(expansion_delimiters(current->value[i + 1]))
			{
				end = i;
				replace_variable(i, end, current);
			}
			i++;
		}
		
		current = current->next;
	}
} */
