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

int check_special_chars(const char *input)
{
    return (ft_strchr(input, '\\') != NULL || ft_strchr(input, ';') != NULL);
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
	if (data->user_input == NULL || data->user_input[0] == '\0'
		|| ft_strncmp(data->user_input, "\n", 1) == 0
			|| whitespace_chars(data->user_input[0]))
		{
			free(data->user_input);
			data->user_input = NULL;
			ft_printf("No input provided.\n");
			return 0;
		}
		return 1;
}

