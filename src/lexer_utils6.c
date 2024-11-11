/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils6.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 17:11:24 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/12 17:11:26 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Libft/libft.h"
#include "../includes/minishell.h"

void	update_token_type_before_processing(t_data *data)
{
	if (data->expect_command)
	{
		data->type = T_COMMAND;
		data->expect_command = 0;
	}
	else
	{
		data->type = T_ARGUMENT;
	}
}

void	process_based_on_type(t_data *data, char *str, size_t *idx)
{
	if (shell_operators(str[*idx]))
	{
		handle_operator(data, str, idx);
		update_expectations(data);
	}
	else
	{
		update_token_type_before_processing(data);
		if (process_chars(data, str, idx, data->type) != 0)
		{
			ft_printf("Failed to tokenize input at index %zu\n", *idx);
			free_tokens(data);
			return ;
		}
	}
}

void	process_input(t_data *data, char *str)
{
	size_t	idx;

	idx = 0;
	data->expect_command = 1;
	data->type = T_COMMAND;
	while (str[idx])
	{
		if (whitespace_chars(str[idx]))
		{
			idx++;
			continue ;
		}
		process_based_on_type(data, str, &idx);
	}
}
