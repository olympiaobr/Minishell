/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_synthax2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 13:38:29 by olobresh          #+#    #+#             */
/*   Updated: 2024/03/30 13:38:30 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

int check_special_chars(const char *input) {
    char curr_quote = 0;  // To keep track of whether we are inside quotes
    int escaped = 0;      // To track if the current character is preceded by a backslash

    while (*input) {
        if (escaped) {
            escaped = 0;  // Reset escape status after skipping one character
        } else if (*input == '\\') {
            escaped = 1;  // Set escaped to true if a backslash is encountered
        } else if ((*input == '\'' || *input == '\"') && !escaped) {
            if (curr_quote == '\0') {  // Not inside quotes
                curr_quote = *input;  // Start quote
            } else if (curr_quote == *input) {
                curr_quote = '\0';  // End quote
            }
        } else if ((curr_quote == '\0') && (*input == ';' || (*input == '\\' && *(input + 1) != '\\'))) {
            return 1;  // Found special character outside of quotes or single backslash
        }
        ++input;
    }
    return 0;  // No illegal characters found outside of quotes
}

int quote_error(const char *input)
{
    char curr_q = '\0';

	while (*input)
	{
        if ((*input == '\'' || *input == '\"'))
		{
            if (curr_q == '\0')
			{
                curr_q = *input;
            }
			else if (curr_q == *input)
			{
                curr_q = '\0';
            }
        }
        ++input;
    }
    if (curr_q != '\0')
	{
        return 1;
    }
	else
        return 0;
}

void count_q(char c, int *sq, int *dq)
{
    if ('\"' == c)
	{
        *dq += 1;
    }
    if ('\'' == c)
	{
        *sq += 1;
    }
}
int validate_input(t_data *data)
{
    int i;

    if (data->user_input == NULL || data->user_input[0] == '\0' || ft_strncmp(data->user_input, "\n", 1) == 0)
	{
        return 0;
    }
    i = 0;
    while (data->user_input[i] != '\0')
	{
        if (!whitespace_chars(data->user_input[i]))
		{
            return 1;
        }
        i++;
    }
    return 0;
}

