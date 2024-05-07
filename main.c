/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 14:39:02 by jasnguye          #+#    #+#             */
/*   Updated: 2024/05/06 16:07:03 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

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
    data->std_input_fd = STDIN_FILENO;
    data->std_output_fd = STDOUT_FILENO;
    data->append = 0;
    data->heredoc = 0;
    initialize_shell_components(data);
}
void catch_sigint(int sig)
{
    (void)sig;
    ft_putchar_fd('\n', STDOUT_FILENO);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void handle_sigquit(int signum)
{
    if (signum == SIGQUIT)
    {
        printf("Quit (core dumped)\n");
        ft_putchar_fd('\n', STDOUT_FILENO);
        rl_on_new_line();
    }
}

void setup_interactive_signals(void)
{
    struct sigaction action_int, action_quit;

    ft_memset(&action_int, 0, sizeof(struct sigaction));
    action_int.sa_handler = catch_sigint;
    sigemptyset(&action_int.sa_mask);
    action_int.sa_flags = 0;
    sigaction(SIGINT, &action_int, NULL);

    ft_memset(&action_quit, 0, sizeof(struct sigaction));
    action_quit.sa_handler = SIG_IGN;
    sigemptyset(&action_quit.sa_mask);
    action_quit.sa_flags = 0;
    sigaction(SIGQUIT, &action_quit, NULL);
}

void setup_noninteractive_signals(void)
{
    struct sigaction action;

    ft_memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = handle_sigquit;
    sigemptyset(&action.sa_mask);
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGQUIT, &action, NULL);
}

// Signal setup caller function based on the mode
void signal_setup(int mode)
{
    if (mode == 1)
    {
        setup_interactive_signals();
    }
    else
    {
        setup_noninteractive_signals();
    }
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
        setup_noninteractive_signals();
        if (validate_input(data))
        {
            add_history(data->user_input);
            lexing_input(data);
            if(data->heredoc == 1)
			{
				data->heredoc = 0;
				check_for_heredoc(data);
				parser(data);
				execution(data);
                //signal
				continue;
			}
            expansion(data);
            parser(data);
            execution(data);
        }
        setup_interactive_signals();
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

