/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_synthax3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 16:16:18 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/10 16:16:20 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

int quote_error(const char *input)
{
    char curr_q;

	curr_q = '\0';
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
