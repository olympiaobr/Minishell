/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 12:42:54 by jasnguye          #+#    #+#             */
/*   Updated: 2024/04/19 14:01:44 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

//if there is << call this function heredoc()
//look what is after << that will be the delimiter
//it then calls a different function that -> > input (infinite loop)
//then we read until the delimiters appears again and we tokenize this input
//error check if there is no second appearance of the delimiter

void write_to_heredoc_file(/* t_data *data, */ char *delimiter)
{
	char *input;
	char *temp_file = "heredoc_tempfile";
	int fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(!fd)
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
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
		//process_input(data, input);//puts input into tokens
	}
	close(fd);
	free(input);
}

void redirect_to_standard_input()
{
	char *temp_file = "heredoc_tempfile";
	int fd = open(temp_file, O_RDONLY);

	if(!fd)
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

}

void check_for_heredoc(t_data *data)
{
	t_token *current = data->token_list;
	while(current != NULL)
	{
		if(current->type == T_HEREDOC)
		{
			char *delimiter = current->next->value;// go to next token and pass it to heredoc function
			write_to_heredoc_file(/* data, */ delimiter);
			redirect_to_standard_input();
			
		}
		current = current->next;
	}
}
