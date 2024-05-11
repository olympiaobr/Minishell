/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 17:27:10 by jasnguye          #+#    #+#             */
/*   Updated: 2024/05/11 20:58:33 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

char	*get_value(char *variable_name, t_data *data)
{
	return (cust_getenv(variable_name, data));
}

int	expansion_delimiters(char c)
{
	return (c == ':' || c == '-' || c == '?' || c == '+' || c == '%'
		|| c == '=' || c == '/' ||  c == '*'
		|| c == '#' || c == '@' || c == '.' || c == '$' || c == ',');
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
        //printf("Expanding: %s, is_quoted: %d\n", current->value, current->is_quoted);

        if (current->is_quoted != 1)
        {
            char *expanded_value = ft_strdup("");
            int i = 0;
            while (current->value[i] != '\0')
            {
                if (current->value[i] == '$')
                {

					if (current->value[0] == '$' && current->value[1] == '?' && ft_strcmp(data->token_list->value, "expr") != 0)
					{
						ft_printf("exit status: %d", data->exit_status);
						break;
					}

					if(current->value[0] == '$' && ft_strcmp(data->token_list->value, "expr") != 0 )
					{
						ft_printf("$"); //edge case
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
///////// not done splitting yet !!


/* void expand_value2(t_data *data, t_token *current, int i, char *expanded_value)
{
	if(current->value[0] == '$' && ft_strcmp(data->token_list->value, "expr") != 0 )
				ft_printf("$"); //edge case
            int start = i + 1;
            while (current->value[start] != '\0' && valid_var_char(current->value[start]))
                    start++;
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


void expand_value1(t_data *data, t_token *current)
{
	char *expanded_value = ft_strdup("");
    int i = 0;
    while (current->value[i] != '\0')
    {
        if (current->value[i] == '$')
    	{
			if (current->value[0] == '$' && current->value[1] == '?' && ft_strcmp(data->token_list->value, "expr") != 0)
			{
				ft_printf("exit status: %d", data->exit_status);
				break;
			}
			expand_value2(data, current, i, expanded_value);
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
 

void expansion(t_data *data)
{
    t_token *current = data->token_list;
    while (current != NULL)
    {//printf("Expanding: %s, is_quoted: %d\n", current->value, current->is_quoted);
        if (current->is_quoted != 1)
        {
           expand_value1(data, current);
        }
        current = current->next;
    }
} */