/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:43:45 by olobresh          #+#    #+#             */
/*   Updated: 2024/03/26 12:43:47 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "includes/minishell.h"

int	whitespace_chars(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

char	*trim_value(const char *val)
{
	return (ft_strtrim(val, " \t\n\v\f\r"));
}

t_token	*init_token(token_type type)
{
	t_token	*t;

	t = malloc(sizeof(t_token));
	if (!t)
		return (NULL);
	t->type = type;
	t->value = NULL;
	t->next = NULL;
	return (t);
}

t_token	*allocate_trimmed_token(token_type type, char *val)
{
	char	*trimmed_val;
	t_token	*token;

	trimmed_val = trim_value(val);
	if (!trimmed_val)
		return (NULL);
	token = init_token(type);
	if (!token)
	{
		free(trimmed_val);
		return (NULL);
	}
	token->value = ft_strdup(trimmed_val);
	if (!token->value)
	{
		free(trimmed_val);
		free(token);
		return (NULL);
	}
	free(trimmed_val);
	return (token);
}

void	append_token(t_token **token_list, t_token *new_token)
{
	t_token	*current;

	if (!token_list || !new_token)
		return ;
	if (!*token_list)
	{
		*token_list = new_token;
		return ;
	}
	current = *token_list;
	while (current->next)
		current = current->next;
	current->next = new_token;
}
