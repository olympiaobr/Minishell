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

void quote_status(char c, int *in_single_quote, int *in_double_quote)
{
    if (c == '\'' && !(*in_double_quote))
    {
        *in_single_quote = !(*in_single_quote);
    }
    else if (c == '\"' && !(*in_single_quote))
    {
        *in_double_quote = !(*in_double_quote);
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
