/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 13:25:19 by olobresh          #+#    #+#             */
/*   Updated: 2024/04/22 16:11:32 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

int	whitespace_chars(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

t_token	*init_token(token_type type, char *value, int is_quoted)
{
	t_token	*t;

	t = malloc(sizeof(t_token));
	if (!t)
		return (NULL);
	t->type = type;
	if (value)
	{
		t->value = ft_strdup(value);
	}
	else
	{
		t->value = NULL;
	}
	t->is_quoted = is_quoted;
	t->next = NULL;
	return (t);
}

t_token	*allocate_token(token_type type, char *val, int is_quoted)
{
	char	*trimmed_val;
	t_token	*token;

	if (is_quoted)
	{
		trimmed_val = ft_strdup(val);
	}
	else
	{
		trimmed_val = trim_value(val);
	}
	if (!trimmed_val)
	{
		return (NULL);
	}
	token = init_token(type, trimmed_val, is_quoted);
	free(trimmed_val);
	if (token == NULL)
		return (NULL);
	return (token);
}

void	append_token(t_token **token_list, t_token *new_token)
{
	t_token	*current;

	if (!token_list || !new_token)
	{
		free(new_token->value);
		free(new_token);
		return ;
	}
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
