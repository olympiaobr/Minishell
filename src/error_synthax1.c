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

#include "../Libft/libft.h"
#include "../includes/minishell.h"

int	is_valid_redirection(const char **curr, int sq, int dq)
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

int	redirection_error(const char *cmd)
{
	int			sq;
	int			dq;
	const char	*curr;

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

int	check_operators(const char *command, int *i, int sq, int dq)
{
	char	current_char;

	current_char = command[*i];
	if ((sq & 1) == 0 && (dq & 1) == 0)
	{
		if ((current_char == '&' && command[*i + 1] == '&')
			|| (current_char == '|' && command[*i + 1] == '|'))
		{
			return (1);
		}
	}
	return (0);
}

int	wrong_operators(const char *command)
{
	int	sq;
	int	dq;
	int	i;
	int	result;

	sq = 0;
	dq = 0;
	i = 0;
	while (command[i] != '\0')
	{
		count_q(command[i], &sq, &dq);
		result = check_operators(command, &i, sq, dq);
		if (result)
			return (1);
		i++;
	}
	return (0);
}
