/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils5.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 16:11:11 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/12 16:11:13 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

int	tokenize_word(t_data *data, char *str, size_t *idx,
		token_type expected_type)
{
	size_t	start_idx;
	int		in_quote;
	char	quote_char;
	size_t	length;
	char	*word;

	start_idx = *idx;
	in_quote = 0;
	quote_char = '\0';
	process_characters(str, idx, &in_quote, &quote_char);
	length = *idx - start_idx;
	if (quote_char && str[start_idx] == quote_char && str[*idx
		- 1] == quote_char && length >= 2)
	{
		start_idx++;
		length -= 2;
	}
	word = determine_word(str, start_idx, length, quote_char);
	if (word)
	{
		return (handle_token_finalization(data, word, quote_char,
				expected_type));
	}
	return (-1);
}

t_token	*get_last_token(t_token *token_list)
{
	t_token	*last_token;

	last_token = token_list;
	while (last_token && last_token->next)
	{
		last_token = last_token->next;
	}
	return (last_token);
}

void update_expectations(t_data *data, int *expect_command, token_type *type)
{
    t_token *last_token = get_last_token(data->token_list);
    if (last_token)
    {
        if (last_token->type == T_HEREDOC)
        {
            *expect_command = 0;
            *type = T_DELIMITER;
        }
        else if (last_token->type == T_PIPE || last_token->type == T_OUT
                 || last_token->type == T_IN || last_token->type == T_APPEND)
        {
            *expect_command = 1;
            *type = T_COMMAND;
        }
        else
        {
            *type = T_ARGUMENT;
        }
    }
}

void	handle_operator(t_data *data, char *str, size_t *idx)
{
	tokenize_operator(data, str, idx);
}

int	process_chars(t_data *data, char *str, size_t *idx, token_type type)
{
	if (whitespace_chars(str[*idx]))
	{
		(*idx)++;
		return (0);
	}
	if (shell_operators(str[*idx]))
	{
		tokenize_operator(data, str, idx);
		return (0);
	}
	else
	{
		return (tokenize_word(data, str, idx, type));
	}
}
