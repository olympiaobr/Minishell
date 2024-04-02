/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 17:27:10 by jasnguye          #+#    #+#             */
/*   Updated: 2024/04/02 14:31:25 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

char *get_value(char *variable_name)
{
	return getenv(variable_name);
}

int	expansion_delimiters(char c)
{
	return (c == ':' || c == '-' || c == '?' || c == '+' || c == '%'
		|| c == '=' || c == '/' ||  c == '*' || c == '#' || c == '@' || c == '.' || c == '$' || c == ',');
}

int valid_var_char(int c)
{
    if (c >= '0' && c <= '9')
    {
        return 1;
    }
    if (c >= 'A' && c <= 'Z')
    {
        return 1;
    }
    if (c >= 'a' && c <= 'z')
    {
        return 1;
    }
    if (c == '_')
    {
        return 1;
    }
    return 0;
}

char *ft_strjoin_char(const char *str, char c)
{
    size_t len = 0;
	size_t i = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    // allocate memory for the new string (original length + 1 for the character + 1 for the null terminator)
    char *result = (char *)malloc((len + 2) * sizeof(char));
    if (result == NULL)
    {
        return NULL;
    }
	while(i < len)
	{
		result[i] = str[i];
		i++;
	}
    result[len] = c;
    result[len + 1] = '\0';

    return (result);
}

void expansion(t_data *data)
{
    t_token *current = data->token_list;
    while (current != NULL)
    {
        int i = 0;
        char *expanded_value = ft_strdup("");

        while (current->value[i] != '\0')
        {
            if (current->value[i] == '$')
            {
                int start = i + 1;
                while (current->value[start] != '\0' && (valid_var_char(current->value[start]) || current->value[start] == '_'))
                {
                    start++;
                }
                char *variable_name = ft_substr(current->value, i + 1, start - (i + 1));
                char *value = get_value(variable_name);
                free(variable_name);
                if (value != NULL)
                {
                    char *temp = expanded_value;
                    expanded_value = ft_strjoin(expanded_value, value);
                    free(temp);
                }
                i = start - 1;
            }
            else
            {
                char *temp = expanded_value;
                expanded_value = ft_strjoin_char(expanded_value, current->value[i]);
                free(temp);
            }
            i++;
        }
        free(current->value);
        current->value = expanded_value;
        current = current->next;
    }
}
