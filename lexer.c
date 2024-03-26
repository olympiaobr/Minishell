/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:32:06 by olobresh          #+#    #+#             */
/*   Updated: 2024/03/26 15:46:41 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/* int	shell_operators(int c)
{
	char	*operators;

	operators = "|><";
	while (*operators)
	{
		if (*operators == c)
			return (1);
		operators++;
	}
	return (0);
}

char	*extract_next_word(char *str)
{
	int	i;
	int	start;

	i = 0;
	while (str[i] && whitespace_chars(str[i]))
		i++;
	start = i;
	while (str[i] && !whitespace_chars(str[i]) && !shell_operators(str[i]))
		i++;
	return (ft_substr(str, start, i - start));
}

int	only_whitespaces(char *str)
{
	int	index;

	index = 0;
	while (str[index] != '\0')
	{
		if (!whitespace_chars(str[index]))
			return (0);
		index++;
	}
	return (1);
} */
