/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 13:25:25 by olobresh          #+#    #+#             */
/*   Updated: 2024/03/26 13:25:27 by olobresh         ###   ########.fr       */
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
