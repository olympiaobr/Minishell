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

// Identifies and tokenizes operators in the input string,
// appending them to the token list.
void	tokenize_operator(t_data *data, char *str, size_t *idx)
{
	size_t		len;
	char		*substr_operator;
	token_type	type;
	
	if (shell_operators(str[*idx]))
	{
		if (shell_operators(str[*idx + 1]))
		{
			len = 2;
		}
		else
		{
			len = 1;
		}
		substr_operator = ft_substr(str, *idx, len);
		if (substr_operator != NULL)
		{
			type = determine_type(substr_operator);
			create_and_append_token(&(data->token_list), substr_operator, type);
			free(substr_operator);
		}
		*idx += len;
	}
}

// Extracts the next word from the input string
//+ tokenizes it as a command or argument.
void	tokenize_word(t_data *data, char *str, size_t *idx,
		int is_command_parsed)
{
	char		*word;
	size_t		word_length;
	token_type	type;

	word = extract_next_word(str + *idx);
	if (word != NULL && *word != '\0')
	{
		if (is_command_parsed)
		{
			type = T_ARGUMENT;
		}
		else
		{
			type = T_COMMAND;
			is_command_parsed = 1;
		}
		create_and_append_token(&(data->token_list), word, type);
		word_length = ft_strlen(word);
		*idx += word_length;
		free(word);
	}
}
// Tokenizes the remaining unprocessed part
// of the input string as a command.
void	tokenize_rest(t_data *data, char *str, size_t *idx, int len)
{
	char	*remaining;

	if (len > 0)
	{
		remaining = ft_substr(str, *idx, len);
		if (remaining != NULL)
		{
			create_and_append_token(&(data->token_list), remaining, T_COMMAND);
			*idx += len;
			free(remaining);
		}
	}
}

void	tokenize_input(t_data *data, char *input)
{
	size_t	idx;
	size_t	input_length;
	int		is_command_parsed;

	idx = 0;
	input_length = ft_strlen(input);
	is_command_parsed = 0;
	while (idx < input_length)
	{
		tokenize_operator(data, input, &idx);
		tokenize_word(data, input, &idx, is_command_parsed);
		is_command_parsed = 1;
		tokenize_rest(data, input, &idx, input_length - idx);
	}
}

/* maybe for another time?
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
