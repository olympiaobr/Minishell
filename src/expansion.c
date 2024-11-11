/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 17:27:10 by jasnguye          #+#    #+#             */
/*   Updated: 2024/05/12 14:25:05 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Libft/libft.h"
#include "../includes/minishell.h"

char	*get_value(char *variable_name, t_data *data)
{
	return (cust_getenv(variable_name, data));
}

int	expansion_delimiters(char c)
{
	return (c == ':' || c == '-' || c == '?' || c == '+' || c == '%' || c == '='
		|| c == '/' || c == '*' || c == '#' || c == '@' || c == '.' || c == '$'
		|| c == ',');
}

int	valid_var_char(int c)
{
	if (c >= '0' && c <= '9')
	{
		return (1);
	}
	if (c >= 'A' && c <= 'Z')
	{
		return (1);
	}
	if (c >= 'a' && c <= 'z')
	{
		return (1);
	}
	if (c == '_')
	{
		return (1);
	}
	return (0);
}

char	*ft_strjoin_char(const char *str, char c)
{
	size_t	len;
	size_t	i;
	char	*result;

	len = 0;
	i = 0;
	while (str[len] != '\0')
	{
		len++;
	}
	result = (char *)malloc((len + 2) * sizeof(char));
	if (result == NULL)
	{
		return (NULL);
	}
	while (i < len)
	{
		result[i] = str[i];
		i++;
	}
	result[len] = c;
	result[len + 1] = '\0';
	return (result);
}

void	check_edge_case(t_data *data, t_token *current)
{
	if (current->value[0] == '$' && ft_strcmp(data->token_list->value,
			"expr") != 0 && (!current->value[1] || current->value[1] == '$'))
	{
		ft_printf("$");
	}
}
