/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:39:02 by jasnguye          #+#    #+#             */
/*   Updated: 2024/04/10 15:45:43 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

int main(int argc, char *argv[], char **envp)
{
	(void)argv;
    t_data data;

	if (argc == 1)
    {
		data = init_data(envp);
		if (!data)
		{
            perror("Failed to initialize shell data structure");
            return EXIT_FAILURE;
        }
        while (1)
		{
            data.user_input = readline("minishell: ");
            data.token_list = NULL;

            if (!data.user_input)
			{
                break;
            }
            if (!validate_input(&data))
			{
                free(data.user_input);
                continue;
            }
			lexing_input(&data);
			expansion(&data);
			parsing(&data);

			t_token *current = data.token_list;
			while (current != NULL)
			{
            	ft_printf("Value: %s Type: ", current->value);
            	if (current->type == T_COMMAND) {
                	ft_printf("T_COMMAND");
            	} else if (current->type == T_PIPE) {
            		ft_printf("T_PIPE");
            	} else if (current->type == T_IN) {
           			ft_printf("T_IN");
            	} else if (current->type == T_OUT) {
           	    	ft_printf("T_OUT");
            	} else if (current->type == T_HEREDOC) {
                	ft_printf("T_HEREDOC");
            	} else if (current->type == T_APPEND) {
                	ft_printf("T_APPEND");
            	} else if (current->type == T_ARGUMENT) {
                	ft_printf("T_ARGUMENT");
            	} else {
                	ft_printf("Unknown Type");
            	}
            	ft_printf("\n");
            	current = current->next;
        	}
			free_tokens(&data);
			free(data.user_input);
    	}
	}
	else
	{
        ft_printf("Non-valid number of arguments!\n");
    }
    return 0;
}



