/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 17:27:10 by jasnguye          #+#    #+#             */
/*   Updated: 2024/04/26 16:30:41 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

char *get_value(char *variable_name, t_data *data)
{
	return cust_getenv(variable_name, data);
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

/*
void check_single_quotes(char *token, t_data *data)
{
	int i = 0;
	int single_quotes = 0;
	while(token[i] != '\0')
	{
		if(token[i] == '\'')
		{
			single_quotes++;
		}
		i++;
	}
	if(single_quotes == 2)
	{
		data->token_list->single_quotes = 1;
	}
	else
	{
		data->token_list->single_quotes = 0;
	}
}
void check_quotes(t_data *data)
{
	t_token *current = data->token_list;
	while(current != NULL)
	{
		check_single_quotes(current->value, data);
		if(data->token_list->single_quotes != 1)
		{
			expansion(data);
		}
		current = current->next;
	}
}
*/

void expansion(t_data *data)
{
    t_token *current = data->token_list;
    while (current != NULL)
    {
        //printf("Expanding: %s, is_quoted: %d\n", current->value, current->is_quoted);

        if (current->is_quoted != 1)
        {
            char *expanded_value = ft_strdup("");
            int i = 0;
            while (current->value[i] != '\0')
            {
				if(current->value[0] == '$')
				{
					ft_printf("$"); // edge case echo $
				}
                if (current->value[i] == '$')
                {
					if(current->value[0] == '$' && current->value[1] == '?')
					{
						ft_printf("exit status: %d", data->exit_status);
						break;
					}
                    int start = i + 1;
                    while (current->value[start] != '\0' && valid_var_char(current->value[start]))
                    {
                        start++;
                    }
                    char *variable_name = ft_substr(current->value, i + 1, start - (i + 1));
                    char *value = get_value(variable_name, data);
                    free(variable_name);
                    if (value)
                    {
                        char *temp = expanded_value;
                        expanded_value = ft_strjoin(expanded_value, value);
                        free(temp);
                    }
                    i = start - 1;
                }
                else
                {
                    char *temp = ft_strjoin_char(expanded_value, current->value[i]);
                    free(expanded_value);
                    expanded_value = temp;
                }
                i++;
            }
            free(current->value);
            current->value = expanded_value;
        }
        current = current->next;
    }
}
