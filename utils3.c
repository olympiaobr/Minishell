/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:16:36 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/13 10:16:38 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

void	free_pipesfd(t_data *data)
{
	int	i;

	if (data->pipesfd != NULL)
	{
		i = 0;
		while (i < data->count_cmd - 1)
		{
			free(data->pipesfd[i]);
			i++;
		}
		free(data->pipesfd);
	}
}

char	*expand_value(char *expanded_value, char *value)
{
	char	*temp;

	if (value)
	{
		temp = expanded_value;
		expanded_value = ft_strjoin(expanded_value, value);
		free(temp);
	}
	return (expanded_value);
}

char	*expand_non_variable(char *expanded_value, char character)
{
	char	*temp;

	temp = ft_strjoin_char(expanded_value, character);
	free(expanded_value);
	return (temp);
}
