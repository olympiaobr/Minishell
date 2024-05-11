/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 12:42:54 by jasnguye          #+#    #+#             */
/*   Updated: 2024/05/11 14:30:13 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/minishell.h"

//if there is << call this function heredoc()
//look what is after << that will be the delimiter
//it then calls a different function that -> > input (infinite loop)
//then we read until the delimiters appears again and we tokenize this input
//error check if there is no second appearance of the delimiter

void write_to_heredoc_file(t_data *data, char *delimiter)
{
    char *input;

    while (1)
    {
        input = readline("> ");
        if (input == NULL)
        {
            continue;
        }
        if (ft_strcmp(input, delimiter) == 0)
        {
            free(input);
            break;
        }

        if (data->heredoc_input == NULL)
        {
            data->heredoc_input = ft_strdup(input);
        }
        else
        {
            char *temp = ft_strjoin(data->heredoc_input, "\n");
            char *new_input = ft_strjoin(temp, input);
            free(temp);
            free(data->heredoc_input);
            data->heredoc_input = new_input;
        }

        free(input);
    }
}

void check_for_output_file(t_data *data)
{
	t_token *current = data->token_list;
	data->output_file = NULL;
	while(current != NULL)
	{
		if(current->type == T_OUT)
		{
			data->output_file = current->next->value;
			data->output_file_present = 1;
		}
		current = current->next;
	}
}
void check_for_heredoc(t_data *data)
{
    t_token *current = data->token_list;
	data->output_file_present = 0;
	check_for_output_file(data);
	printf("output file is: %s\n", data->output_file);
    while (current != NULL)
	{
        if (current->type == T_HEREDOC)
		{
            char *delimiter = current->next->value;
            data->heredoc = 1;
            write_to_heredoc_file(data, delimiter);
        }
        current = current->next;
    }
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

