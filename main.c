/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:39:02 by jasnguye          #+#    #+#             */
/*   Updated: 2024/04/24 13:00:56 by jasnguye         ###   ########.fr       */
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
void reset_shell_state(t_data *data, char **envp)
{
    free(data->user_input);
    data->user_input = NULL;

    t_token *current_token = data->token_list;
    while (current_token) {
        t_token *next_token = current_token->next;
        free(current_token->value);
        free(current_token);
        current_token = next_token;
    }
    data->token_list = NULL;

    free_commands(data->commands);
    data->commands = NULL;

    free(data->input_file);
    data->input_file = NULL;
    free(data->output_file);
    data->output_file = NULL;

    data->std_input_fd = STDIN_FILENO;
    data->std_output_fd = STDOUT_FILENO;
    data->append = 0;
    data->heredoc = 0;
    initialize_shell_components(data);
    if (data->env == NULL)
    {
        if (init_env(data, envp) == EXIT_FAILURE)
        {
            fprintf(stderr, "Failed to reinitialize environment variables.\n");
            exit(EXIT_FAILURE);
        }
    }
}


int main(int argc, char *argv[], char **envp)
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
            add_history(data->user_input);
            lexing_input(data);
			printf("data.heredoc: %d\n", data->heredoc);
			if(data->heredoc == 1)
			{
				data->heredoc = 0;
				check_for_heredoc(data);
				parser(data);
				execution(data);
				continue;
			}
			//check_for_heredoc(data);
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
            	} else if (current->type == T_DELIMITER) {
                	ft_printf("T_DELIMITER");
            	} else if (current->type == T_ARGUMENT){
					ft_printf("T_ARGUMENT");
				} else {
                	ft_printf("Unknown Type");
            	}
            	ft_printf("\n");
            	current = current->next;
        	}
            reset_shell_state(data, envp);
    	}
	}
	else
	{
        ft_printf("Non-valid number of arguments!\n");
    }
    free_all(data);
	free(data);
	return (EXIT_SUCCESS);
}



