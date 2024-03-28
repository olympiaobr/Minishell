/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 13:25:25 by olobresh          #+#    #+#             */
/*   Updated: 2024/03/27 13:47:51 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void	quote_status(char c, int *in_single_q, int *in_double_q)
{
	if (c == '\'' && !(*in_double_q))
	{
		*in_single_q = !(*in_single_q);
	}
	else if (c == '\"' && !(*in_single_q))
	{
		*in_double_q = !(*in_double_q);
	}
}

int	create_and_append_token(t_token **token_list, char *input, token_type type)
{
	t_token	*new_token;

	new_token = allocate_token(type, input);
	if (new_token == NULL)
	{
		return (-1);
	}
	append_token(token_list, new_token);
	return (0);
}

// Identifies and tokenizes operators in the input string,
// appending them to the token list.
void tokenize_operator(t_data *data, char *str, size_t *idx)
{
    size_t operator_len = 1;  // Default length for a single-character operator.
    token_type type;

    // Check if the next character is also an operator to handle multi-character operators.
    if (shell_operators(str[*idx + 1])) {
        operator_len = 2;  // Adjust for a two-character operator.
    }

    // Extract the operator string based on calculated length.
    char *operator_str = ft_substr(str, *idx, operator_len);

    // Determine the operator's type.
    type = determine_type(operator_str);

    // Create and append the token with the determined type.
    create_and_append_token(&data->token_list, operator_str, type);

    // Free the extracted operator string.
    free(operator_str);

    // Increment idx by the length of the operator to move past it in the input string.
    *idx += operator_len;
}
// Extracts the next word from the input string
//+ tokenizes it as a command or argument.
void	tokenize_word(t_data *data, char *str, size_t *idx)
{
	char *word = extract_next_word(str + *idx);
	if (word && word[0] != '\0')
	{
		create_and_append_token(&data->token_list, word, T_COMMAND);
		*idx += ft_strlen(word);
	}
	if (word)
	{
		free(word);
	}
}

void	tokenize_rest(t_data *data, char *str, size_t *idx)
{
	size_t	remaining_len;
	char	*remaining_str;

	remaining_len = ft_strlen(str + *idx);
	if (remaining_len > 0)
	{
		remaining_str = ft_substr(str, *idx, remaining_len);
		create_and_append_token(&data->token_list, remaining_str, T_COMMAND);
		free(remaining_str);
		*idx += remaining_len;
	}
}

void process_input(t_data *data, char *str)
{
    size_t idx = 0;
    size_t str_len = ft_strlen(str);

    while (idx < str_len)
	{
        // Skip leading whitespace
        while (idx < str_len && whitespace_chars(str[idx]))
		{
            idx++;
        }
        if (idx >= str_len)
		{
            break;
        }

        if (shell_operators(str[idx]))
		{
            tokenize_operator(data, str, &idx);
        } else
		{
            tokenize_word(data, str, &idx);
        }
	}
}

token_type	determine_type(char *operator)
{
	int	result;

	result = ft_strncmp(operator, "|", 1);
	if (result == 0)
		return (T_PIPE);
	result = ft_strncmp(operator, "<", 1);
	if (result == 0)
	{
		result = ft_strncmp(operator, "<<", 2);
		if (result == 0)
			return (T_HEREDOC);
		return (T_IN);
	}
	result = ft_strncmp(operator, ">", 1);
	if (result == 0)
	{
		result = ft_strncmp(operator, ">>", 2);
		if (result == 0)
			return (T_APPEND);
		return (T_OUT);
	}
	return (T_COMMAND);
}
/*
 maybe for another time?
 Example: handling command input "ls -la /home",
function could be used to create a token for "ls",
treating it as a command (T_COMMAND).
handles memory allocation
for the extracted command segment and appends the new token
directly to the token list within the `t_data` structure

void	command_token(char **begin, char **end, t_data *data)
{
	char	*segment;
	t_token	*new_cmd;

	if (*end > *begin)
	{
		segment = ft_substr(*begin, 0, *end - *begin);
		if (segment)
		{
			new_cmd = allocate_token(T_COMMAND, segment);
			if (new_cmd != NULL)
			{
				append_token(&(data->token_list), new_cmd);
			}
			else
			{
				ft_putstr_fd("Error: Token creation failed.\n", STDERR_FILENO);
			}
			free(segment);
		}
		else
		{
			ft_putstr_fd("Error: Memory allocation for segment failed.\n",
				STDERR_FILENO);
		}
	}
}
*/
