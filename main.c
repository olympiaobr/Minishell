/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:39:02 by jasnguye          #+#    #+#             */
/*   Updated: 2024/05/12 18:38:44 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

void reset_shell_state(t_data *data)
{
    free(data->user_input);
    data->user_input = NULL;

    free_shell_resources(data);

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

    initialize_shell_components(data);
}

void shell_input(t_data *data)
{
    if (validate_input(data))
    {
        add_history(data->user_input);
        lexing_input(data);
        setup_noninteractive_signals();
        if (data->heredoc == 1)
        {
            data->heredoc = 0;
            data->output_file_present = 0;
            check_for_heredoc(data);
            parser(data);
            execution(data);
            setup_interactive_signals();
            reset_shell_state(data);
            return;
        }
        expansion(data);
        parser(data);
        execution(data);
        setup_interactive_signals();
    }
    reset_shell_state(data);
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
        shell_input(data);
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
