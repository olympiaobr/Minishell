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

void	quote_status(char c, int *in_quote, char *quote_char)
{
	if (!(*in_quote) && (c == '\'' || c == '\"'))
	{
		*in_quote = 1;
		*quote_char = c;
	}
	else if (*in_quote && c == *quote_char)
	{
		*in_quote = 0;
	}
}

int	create_and_append_token(t_token **token_list, char *input, token_type type, int is_quoted)
{
	t_token	*new_token;

	new_token = allocate_token(type, input, is_quoted); // Updated to pass is_quoted
	if (new_token == NULL)
	{
		return (-1);
	}
	append_token(token_list, new_token);
	return (0);
}

token_type	determine_type(char *operator)
{
	if (ft_strncmp(operator, "|", 1) == 0)
		return (T_PIPE);
	if (ft_strncmp(operator, "<<", 2) == 0)
		return (T_HEREDOC);
	if (ft_strncmp(operator, ">>", 2) == 0)
		return (T_APPEND);
	if (ft_strncmp(operator, "<", 1) == 0)
		return (T_IN);
	if (ft_strncmp(operator, ">", 1) == 0)
		return (T_OUT);
	return (T_COMMAND);
}

// Identifies and tokenizes operators in the input string,
// appending them to the token list.
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
	create_and_append_token(&data->token_list, operator_str, type, 0);
	free(operator_str);
	*idx += operator_len;
}

// Extracts the next word from the input string
//+ tokenizes it as a command or argument.
void tokenize_word(t_data *data, char *str, size_t *idx, token_type expected_type)
{
    size_t start_idx = *idx;
    int in_quote = 0;
    char quote_char = '\0';
    while (str[*idx] && (in_quote || (!whitespace_chars(str[*idx]) && !shell_operators(str[*idx]))))
	{
        quote_status(str[*idx], &in_quote, &quote_char);
        (*idx)++;
    }
    size_t length = *idx - start_idx;

    int is_quoted = quote_char != '\0';
    if (!in_quote && quote_char)
	{
        start_idx++;
        length -= 2;
    }
    char *word = ft_substr(str, start_idx, length);
    create_and_append_token(&data->token_list, word, expected_type, is_quoted);
    free(word);
}

void	process_input(t_data *data, char *str)
{
	size_t	idx;
	int		expect_command;
	token_type type;
	t_token	*last_token;

	idx = 0;
	expect_command = 1;
	while (str[idx])
	{
		if (whitespace_chars(str[idx]))
		{
			idx++;
		}
		else
		{
			if (shell_operators(str[idx]))
			{
				tokenize_operator(data, str, &idx);
				if (data->token_list->type == T_PIPE
					|| data->token_list->type == T_OUT
					|| data->token_list->type == T_IN
					|| data->token_list->type == T_APPEND
					|| data->token_list->type == T_HEREDOC)
				{
					expect_command = 1;
				}
			}
			else
			{
				if (expect_command)
				{
					type = T_COMMAND;
					expect_command = 0;
				}
				else
				{
					type = T_ARGUMENT;
				}
				tokenize_word(data, str, &idx, type);
			}
		}
		last_token = data->token_list;
		while (last_token && last_token->next)
		{
			last_token = last_token->next;
		}
		if (last_token && last_token->type == T_PIPE)
		{
			expect_command = 1;
		}
	}
}

/*
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
*/

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
