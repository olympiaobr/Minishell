/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_synthax2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 13:38:29 by olobresh          #+#    #+#             */
/*   Updated: 2024/03/30 13:38:30 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Libft/libft.h"
#include "../includes/minishell.h"

int	check_operator_logic(char ch, int *op, int sq, int dq)
{
	if (ch == '|' && !(sq & 1) && !(dq & 1))
	{
		if (*op == 1)
			return (1);
		*op = 1;
	}
	else if (!whitespace_chars(ch))
	{
		*op = 0;
	}
	return (0);
}

int	error_operators(const char *str)
{
	int	sq;
	int	dq;
	int	op;
	int	result;

	sq = 0;
	dq = 0;
	op = 0;
	if (*str == '|' || *str == '&')
		return (1);
	while (*str)
	{
		count_q(*str, &sq, &dq);
		result = check_operator_logic(*str, &op, sq, dq);
		if (result == 1)
			return (1);
		str++;
	}
	if (op == 1)
		return (1);
	return (0);
}

int	input_check(const char *input)
{
	if (redirection_error(input))
	{
		ft_putstr_fd("Error: Incorrect redirection syntax.\n", STDERR_FILENO);
		return (1);
	}
	if (quote_error(input))
	{
		ft_putstr_fd("Error: Quotes not closed.\n", STDERR_FILENO);
		return (1);
	}
	if (wrong_operators(input))
	{
		ft_putstr_fd("Error: '&&' and '||' operators not allowed.\n",
			STDERR_FILENO);
		return (1);
	}
	if (error_operators(input))
	{
		ft_putstr_fd("Error: Operator used incorrectly.\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

void	process_char(const char **input, char *curr_quote, int *escaped)
{
	if (*escaped)
	{
		*escaped = 0;
	}
	else if (**input == '\\')
	{
		*escaped = 1;
	}
	else if ((**input == '\'' || **input == '\"') && !*escaped)
	{
		if (*curr_quote == '\0')
		{
			*curr_quote = **input;
		}
		else if (*curr_quote == **input)
		{
			*curr_quote = '\0';
		}
	}
}

int	check_special_chars(const char *input)
{
	char	curr_quote;
	int		escaped;

	curr_quote = 0;
	escaped = 0;
	while (*input)
	{
		process_char(&input, &curr_quote, &escaped);
		if (curr_quote == '\0' && (*input == ';' || (*input == '\\' && *(input
						+ 1) != '\\')))
		{
			return (1);
		}
		input++;
	}
	return (0);
}
