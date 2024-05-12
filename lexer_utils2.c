/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 13:25:25 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/03 17:18:47 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	create_and_append_token(t_token **token_list, char *input, token_type type,
		int is_quoted)
{
	t_token	*new_token;

	new_token = allocate_token(type, input, is_quoted);
	if (new_token == NULL)
	{
		return (-1);
	}
	append_token(token_list, new_token);
	return (0);
}

void	tokenize_operator(t_data *data, char *str, size_t *idx)
{
	size_t		operator_len;
	token_type	type;
	char		*operator_str;

	operator_len = 1;
	if (shell_operators(str[*idx + 1]))
	{
		operator_len = 2;
	}
	operator_str = ft_substr(str, *idx, operator_len);
	type = determine_type(operator_str);
	if (type == T_APPEND)
	{
		data->append = 1;
	}
	if (type == T_HEREDOC)
	{
		data->heredoc = 1;
	}
	create_and_append_token(&data->token_list, operator_str, type, 0);
	free(operator_str);
	*idx += operator_len;
}

void	update_quote_state(int *in_quote, char *quote_char, char current_char)
{
	if (!*in_quote)
	{
		*in_quote = 1;
		*quote_char = current_char;
	}
	else
	{
		*in_quote = 0;
		*quote_char = '\0';
	}
}

void	parse_quotes(char *str, char *result, int *index)
{
	int		j;
	int		in_quote;
	char	quote_char;

	j = 0;
	in_quote = 0;
	quote_char = '\0';
	while (str[j] != '\0')
	{
		if ((!in_quote && (str[j] == '\'' || str[j] == '\"')) || (in_quote
				&& str[j] == quote_char))
		{
			update_quote_state(&in_quote, &quote_char, str[j]);
		}
		else
		{
			result[*index] = str[j];
			(*index)++;
		}
		j++;
	}
}

char	*edge_case(char *str)
{
	size_t	len;
	char	*result;
	int		i;

	len = strlen(str);
	result = malloc(len + 1);
	if (!result)
	{
		return (NULL);
	}
	i = 0;
	parse_quotes(str, result, &i);
	result[i] = '\0';
	return (result);
}
