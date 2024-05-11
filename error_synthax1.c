/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_synthax1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 15:42:12 by olobresh          #+#    #+#             */
/*   Updated: 2024/03/29 15:42:16 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

int is_valid_redirection(const char **curr, int sq, int dq)
{
    if (((sq & 1) == 0) && ((dq & 1) == 0) && (**curr == '>' || **curr == '<'))
    {
        return (validate_operator_sequence(curr));
    }
    else
    {
        if (**curr != '\0')
            (*curr)++;
        return (0);
    }
}

int redirection_error(const char *cmd)
{
    int sq;
    int dq;
    const char *curr;

    sq = 0;
    dq = 0;
    curr = cmd;
    while (*curr)
    {
        count_q(*curr, &sq, &dq);
        if (is_valid_redirection(&curr, sq, dq))
        {
            return (1);
        }
    }
    return (0);
}

int	validate_operator_sequence(const char **line)
{
	int	i;

	i = 0;
    if (!*line || !**line)
      return (0);
	if (shell_operators((*line)[i + 1]))
		i += 2;
	else
		i++;
	while ((*line)[i] && ((*line)[i] == ' ' || (*line)[i] == '\t'))
		i++;
	if ((*line)[i] != '\0')
    {
        *line += i;
    }
    else
    {
        *line += i - 1;
    }
	if (!**line || shell_operators(**line))
		return (1);
	return (0);
}

int wrong_operators(const char *command)
{
    int sq = 0;
    int dq = 0;
    char current_char;
    int i = 0;

    while (command[i] != '\0')
	{
        current_char = command[i];
        count_q(current_char, &sq, &dq);
        if ((sq & 1) == 0 && (dq & 1) == 0)
		{
            if ((current_char == '&' && command[i + 1] == '&') ||
                (current_char == '|' && command[i + 1] == '|'))
				{
					return 1;
				}
        }
        i++;
    }
    return 0;
}

int error_operators(const char *str)
{
    int sq = 0;
    int dq = 0;
    int op = 0;

    if (*str == '|' || *str == '&')
        return 1;

    while (*str)
	{
        count_q(*str, &sq, &dq);
        if (*str == '|' && !(sq & 1) && !(dq & 1))
		{
            if (op == 1)
                return 1;
            op = 1;
        }
		else if (!whitespace_chars(*str))
		{
            op = 0;
        }
        str++;
    }
    if (op == 1)
        return 1;
    return 0;
}

int input_check(const char *input)
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
        ft_putstr_fd("Error: '&&' and '||' operators not allowed.\n", STDERR_FILENO);
        return (1);
    }
    if (error_operators(input))
	{
        ft_putstr_fd("Error: Operator used incorrectly.\n", STDERR_FILENO);
        return (1);
    }
    return (0);
}

