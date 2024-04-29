/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 12:42:54 by jasnguye          #+#    #+#             */
/*   Updated: 2024/04/29 15:51:47 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

void write_to_heredoc_file(t_data *data, char *delimiter)
{
	char *input;
	char *temp_file = "heredoc_tempfile";
	int fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(fd == -1)
	{
		perror("failure opening file\n");
		exit(EXIT_FAILURE);
	}
	while(1)
	{
		input = readline("> ");
		if(input && ft_strcmp(input, delimiter) == 0)
		{
			break;
		}
		if(data->heredoc_input == NULL)
		{
			data->heredoc_input = ft_strdup(input);
			//data->heredoc_input = ft_strcat(data->heredoc_input, "\n");
		}
		else
		{
			char *temp = ft_strjoin(data->heredoc_input, input);
			free(data->heredoc_input);
			data->heredoc_input = temp;
			//data->heredoc_input = ft_strcat(data->heredoc_input, "\n");
		}
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
		free(input);
	}
	close(fd);
	
}

/* void redirect_to_standard_input() //not used
{
	char *temp_file = "heredoc_tempfile";
	int fd = open(temp_file, O_RDONLY);

	if(fd == -1)
	{
		perror("failure opening file\n");
		exit(EXIT_FAILURE);
	}

	
	if (dup2(fd, STDIN_FILENO) == -1) //redirects	
	{
        perror("Error duplicating file descriptor");
		close(fd);
        exit(EXIT_FAILURE);
    }
	
	close(fd);

} */

void check_for_heredoc(t_data *data)
{
	t_token *current = data->token_list;
	while(current != NULL)
	{
		if(current->type == T_HEREDOC)
		{
			char *delimiter = current->next->value;// go to next token and pass it to heredoc function
			data->heredoc = 1;
			write_to_heredoc_file(data, delimiter);
			//redirect_to_standard_input(); // had to remove this
			
		}
		current = current->next;
	}
}
