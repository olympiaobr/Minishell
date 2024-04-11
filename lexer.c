/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:32:06 by olobresh          #+#    #+#             */
/*   Updated: 2024/04/10 15:41:42 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

char	*trim_value(const char *val)
{
	return (ft_strtrim(val, " \t\n\v\f\r"));
}

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
    char *trimmed_input;

    trimmed_input = trim_value(data->user_input);
    if (!trimmed_input)
	{
        ft_printf("Error: Memory allocation failed for trimming.\n");
        return;
    }
    free(data->user_input);
    data->user_input = trimmed_input;
    if (check_special_chars(data->user_input))
	{
        ft_printf("Error: Special characters such as '\\' or ';' not allowed.\n");
        return;
    }
    if (input_check(data->user_input))
	{
        return;
    }
    process_input(data, data->user_input);
    if (data->token_list == NULL)
	{
        ft_printf("No tokens generated.\n");
    }
}
