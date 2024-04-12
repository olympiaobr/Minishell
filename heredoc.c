/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 12:42:54 by jasnguye          #+#    #+#             */
/*   Updated: 2024/04/12 14:08:10 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

//if there is << call this function heredoc()
//look what is after << that will be the delimiter
//it then calls a different function that -> > input (infinite loop)
//then we read until the delimiters appears again and we tokenize this input
//error check if there is no second appearance of the delimiter

//still need to make the heredoc input an T_Argument instead of a T_Command otherwise the check_command function won't work properly
void heredoc(t_data *data, char *delimiter)
{
	char *input;
	while(1)
	{
		input = readline("> ");
		if(input && ft_strcmp(input, delimiter) == 0)
		{
			break;
			free(input);
		}
		process_input(data, input);//puts input into tokens
		
	}
	free(input);
}

void check_for_heredoc(t_data *data)
{
	t_token *current = data->token_list;
	while(current != NULL)
	{
		if(current->type == T_HEREDOC)
		{
			char *delimiter = current->next->value;// go to next token and pass it to heredoc function
			heredoc(data, delimiter);
		}
		current = current->next;
	}
}
