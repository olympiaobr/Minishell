/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 14:25:39 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/12 14:25:40 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

void	process_characters(char *str, size_t *idx, int *in_quote,
		char *quote_char)
{
	while (str[*idx])
	{
		if (str[*idx] == '\\' && *in_quote && *quote_char == '\"' && (str[*idx
					+ 1] == '$' || str[*idx + 1] == '\"'
				|| str[*idx + 1] == '\\'))
		{
			(*idx) += 2;
			continue ;
		}
		quote_status(str[*idx], in_quote, quote_char);
		if (!*in_quote && (whitespace_chars(str[*idx])
				|| shell_operators(str[*idx])))
		{
			break ;
		}
		(*idx)++;
	}
}

char	*determine_word(char *str, size_t start_idx, size_t length,
		char quote_char)
{
	char	*word;
	char	*temp;

	if (quote_char && str[start_idx] == quote_char && str[start_idx + length
			- 1] == quote_char)
	{
		start_idx++;
		length -= 2;
	}
	word = ft_substr(str, start_idx, length);
	if (quote_char && str[start_idx - 1] != quote_char && str[start_idx
			+ length] != quote_char)
	{
		temp = edge_case(word);
		free(word);
		return (temp);
	}
	return (word);
}

int	set_quoted_flag(char quote_char)
{
	if (quote_char == '\'')
	{
		return (1);
	}
	else if (quote_char == '\"')
	{
		return (2);
	}
	return (0);
}

void	adjust_indices(size_t *start_idx, size_t *length, char quote_char,
		char *str)
{
	if (quote_char && str[*start_idx] == quote_char && str[*start_idx + *length
			- 1] == quote_char)
	{
		(*start_idx)++;
		(*length) -= 2;
	}
}

int	handle_token_finalization(t_data *data, char *word, char quote_char,
		token_type expected_type)
{
	int	is_quoted;

	is_quoted = set_quoted_flag(quote_char);
	printf("Tokenizing: %s, is_quoted: %d\n", word, is_quoted);
	if (create_and_append_token(&data->token_list, word, expected_type,
			is_quoted) != 0)
	{
		free(word);
		return (-1);
	}
	free(word);
	return (0);
}
