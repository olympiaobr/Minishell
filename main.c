/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:39:02 by jasnguye          #+#    #+#             */
/*   Updated: 2024/05/11 20:12:16 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

volatile sig_atomic_t heredoc_mode = 0;

void reset_shell_state(t_data *data)
{
    free(data->user_input);
    data->user_input = NULL;
    t_token *current_token = data->token_list;
    while (current_token)
    {
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
    if (data->heredoc_input != NULL)
    {
        free(data->heredoc_input);
        data->heredoc_input = NULL;
    }
    data->std_input_fd = STDIN_FILENO;
    data->std_output_fd = STDOUT_FILENO;
    data->append = 0;
    data->heredoc = 0;
	heredoc_mode = 0;
	heredoc_interrupted_flag = 0;
    initialize_shell_components(data);
}

void run_shell(t_data *data)
{
    while (1)
    {
        setup_interactive_signals();
        data->user_input = readline("minishell: ");
        if (!data->user_input)
        {
            printf("exit\n");
            break;
        }
        if (validate_input(data))
        {
            add_history(data->user_input);
            lexing_input(data);
            setup_noninteractive_signals();
			
            if(data->heredoc == 1)
			{
				heredoc_signals();
				heredoc_mode = 1;
				if (heredoc_interrupted_flag == 1)
				{
					//check_for_heredoc(data);
                	reset_shell_state(data);
					flush_output();
                	continue;
            	}
				data->heredoc = 0;
				data->output_file_present = 0;
				check_for_heredoc(data);
				parser(data);
				execution(data);
                //signal handling for heredoc here?
                reset_shell_state(data);
				continue;
			}
            expansion(data);
            parser(data);
            execution(data);
            setup_interactive_signals();
        }
        reset_shell_state(data);
        free(data->user_input);
    }
}

int main(int argc, char *argv[], char **envp)
{
    if (argc != 1)
    {
        ft_printf("Usage: %s\n", argv[0]);
        return (EXIT_FAILURE);
    }
    t_data *data = init_data(envp);
    if (!data)
    {
        perror("Failed to initialize shell data structure");
        return (EXIT_FAILURE);
    }
    run_shell(data);
    free_all(data);
    free(data);
    return (EXIT_SUCCESS);
}

/*void display_commands(t_data *data)
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
*/

