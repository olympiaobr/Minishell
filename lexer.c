/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:32:06 by olobresh          #+#    #+#             */
/*   Updated: 2024/03/27 16:00:07 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

 int	shell_operators(int c)
{
	char	*operators;

	operators = "|><";
	while (*operators)
	{
		if (*operators == c)
			return (1);
		operators++;
	}
	return (0);
}

char	*extract_next_word(char *str)
{
	int	i;
	int	start;

	i = 0;
	while (str[i] && whitespace_chars(str[i]))
		i++;
	start = i; 
	while (str[i] && !whitespace_chars(str[i]) /* && !shell_operators(str[i]) */)
		i++;
	return (ft_substr(str, start, i - start));
}

int	only_whitespaces(char *str)
{
	int	index;

	index = 0;
	while (str[index] != '\0')
	{
		if (!whitespace_chars(str[index]))
			return (0);
		index++;
	}
	return (1);
} 



void lexing_input(t_data *data)
{
	char *input_string = data->user_input;
	int i = 0;
	char *token;

	while(input_string[i] != '\0')
	{		
		while (input_string[i] == ' ' || input_string[i] == '\t' || input_string[i] == '\n') // whitespace_chars function can be implemented here 
		{
            i++;
    	}
			token = extract_next_word(input_string + i);
			t_token *new_token = (t_token *)malloc(sizeof(t_token));
        	if (new_token == NULL) 
			{
            	printf("Error: Memory allocation failed\n");
            	exit(EXIT_FAILURE);
       		}
			new_token->value = ft_strdup(token);
			new_token->next = NULL;
			if (data->token_list == NULL)
			{
				data->token_list = new_token;
			}
			//type
			else
			{
				t_token *current = data->token_list;
				while(current->next != NULL)
				{
					current = current->next;
				}
				current->next = new_token;
			}
			while(input_string[i] == ' '|| input_string[i] == '\t' || input_string[i] == '\n') // whitespace_chars function can be implemented here 
			{
				i++;
			}
			i += ft_strlen(token);
			free(token);
			
	}
} 