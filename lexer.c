/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:32:06 by olobresh          #+#    #+#             */
/*   Updated: 2024/03/27 12:14:29 by jasnguye         ###   ########.fr       */
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
	while (str[i] && !whitespace_chars(str[i]) && !shell_operators(str[i]))
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
	
	while (input_string[i] != '\0' && !only_whitespaces(input_string))
	{
		while(whitespace_chars(input_string[i]))
		{
			i++;
		}
		token = extract_next_word(input_string + i);
		//init_token(data->token_list->type);
		tokenize_input(data, token);
		ft_printf("token %s\n", token);
		printf("tokenized: %s\n", data->token_list->value);

		i += strlen(token); 
		 //free token
	}
		
}