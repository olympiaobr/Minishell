/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:39:02 by jasnguye          #+#    #+#             */
/*   Updated: 2024/04/15 15:10:33 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

void display_commands(t_data *data)
{
    t_command *cmd = data->commands;
    int i = 0;

    while (cmd != NULL)
    {
        ft_printf("\nCommand %d: %s\n", ++i, cmd->command);
        t_token *arg = cmd->argv;
        while (arg != NULL)
        {
            ft_printf("\tArgument: %s\n", arg->value);
            arg = arg->next;
        }
        t_token *opt = cmd->option;
        while (opt != NULL)
        {
            ft_printf("\tOption: %s\n", opt->value);
            opt = opt->next;
        }
        cmd = cmd->next;
    }
    if (data->input_file)
        ft_printf("\nInput Redirection: %s\n", data->input_file);
    if (data->output_file)
        ft_printf("Output Redirection: %s\n", data->output_file);
}


int main(int argc, char *argv[], char **envp )
{
    (void)argv;
    t_data *data;
	//t_command *cmd = NULL;
 	data = init_data(envp);
    if (argc == 1)
    {
       
        if (!data)
        {
            perror("Failed to initialize shell data structure");
            return EXIT_FAILURE;
        }
        while (1)
        {
            data->user_input = readline("minishell: ");
            data->token_list = NULL;

            if (!data->user_input)
            {
                break;
            }
            if (!validate_input(data))
            {
                free(data->user_input);
				data->user_input = NULL;
                continue;
            }
            lexing_input(data);
			check_for_heredoc(data);
            expansion(data);
            parser(data);  
			execution(data/* , cmd */);

            t_token *current = data->token_list;
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
			display_commands(data);
			free(data->user_input);
			data->user_input = NULL;
			free_all(data);
			ft_memset(data, 0, sizeof(*data));
			initialize_shell_components(data);
    	}
	}
	else
	{
        ft_printf("Non-valid number of arguments!\n");
    }
	free(data);
	return (EXIT_SUCCESS);
}



