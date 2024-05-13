/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:21:34 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/13 10:21:37 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

int	find_start(t_token *current, int start)
{
	while (current->value[start] != '\0'
		&& valid_var_char(current->value[start]))
	{
		start++;
	}
	return (start);
}

int	find_end_of_variable(t_token *token, int start_index)
{
	int	i;

	i = start_index;
	while (token->value[i] != '\0' && token->value[1] == '?')
	{
		i++;
	}
	return (i);
}

void	handle_variable(t_data *data, t_token **current, char **expanded_value,
		int *i)
{
	int		start;
	char	*variable_name;
	char	*value;

	if ((*current)->value[0] == '$' && (*current)->value[1] == '?'
		&& ft_strcmp(data->token_list->value, "expr") != 0)
	{
		ft_printf("exit status: %d", data->exit_status);
		*i = find_end_of_variable(*current, *i) - 1;
	}
	else
	{
		check_edge_case(data, (*current));
		start = *i + 1;
		start = find_start((*current), start);
		variable_name = ft_substr((*current)->value, *i + 1, start - (*i + 1));
		value = get_value(variable_name, data);
		free(variable_name);
		*expanded_value = expand_value(*expanded_value, value);
		*i = start - 1;
	}
}

void	expand_token(t_data *data, t_token **current, char **expanded_value)
{
	int	i;

	i = 0;
	while ((*current)->value[i] != '\0')
	{
		if ((*current)->value[i] == '$')
		{
			handle_variable(data, current, expanded_value, &i);
		}
		else
		{
			*expanded_value = expand_non_variable(*expanded_value,
					(*current)->value[i]);
		}
		i++;
	}
}

void	expansion(t_data *data)
{
	t_token	*current;
	char	*expanded_value;

	current = data->token_list;
	while (current != NULL)
	{
		if (current->is_quoted != 1)
		{
			expanded_value = ft_strdup("");
			expand_token(data, &current, &expanded_value);
			free(current->value);
			current->value = expanded_value;
		}
		current = current->next;
	}
}
