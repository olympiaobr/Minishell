/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 17:27:10 by jasnguye          #+#    #+#             */
/*   Updated: 2024/04/02 15:45:44 by jasnguye         ###   ########.fr       */
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
int check_double_quotes(t_data *data)
{
	t_token *current = data->token_list;
	while(current != NULL)
	{
		int i = 0;
		int double_quotes = 0;
		while(current->value[i] != '\0')
		{
			if(current->value[i] == '"')
			{
				double_quotes ==
			}
			i++;
		
		}
		
		current = current->next;

	}
	
} */
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
                int j = 0;
                while ((current->value[j + 1] != '\0' && !expansion_delimiters(current->value[j + 1])) || (current->value[i + 1] != '\0'&& !expansion_delimiters(current->value[i + 1])))
                {
                    i++;
                    j++;
                }
                int end = start + j;  
                char *variable_name = ft_substr(current->value, start, end - start);// Extract the variable name    	
                char *value = get_value(variable_name); 
				ft_printf("start: %d end: %d\n", start, end);
               	free(variable_name);
                if (value != NULL)
                {
                    expanded_value = ft_strjoin(expanded_value, value);
                    while (current->value[start] != '\0' && expansion_delimiters(current->value[start]))
                    {
                        expanded_value = ft_strjoin_char(expanded_value, current->value[start]);
                        start++;
                    }
                    i = j + start;
                }
				  else
                {
					i++;
                }
            }
            else //if it's not a variable
            {
                expanded_value = ft_strjoin_char(expanded_value, current->value[i]);
				i++;
            }
        }
        free(current->value); // Free the previous value
        current->value = expanded_value;// Update the value of the token with expanded_value
        current = current->next;
    }
} 
