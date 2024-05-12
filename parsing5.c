/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 13:01:01 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/12 13:01:03 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

// Determines the correct file open flags based on the token's type and calls setup_redirection to apply these settings.
int apply_redirection(t_data *data, t_token *token)
{
    char *file_name = NULL;
    if (data->output_file_present == 1)
    {
        file_name = ft_strdup(data->output_file);
        printf("here output file present\n");
    }
    else if (data->heredoc == 1 && token->type == T_HEREDOC)
    {
        file_name = ft_strdup("heredoc_tempfile");
    }
    else
    {
        file_name = ft_strdup(token->next->value);
    }

    int result = setup_redirection(data, token, file_name);
    free(file_name);
    return result;
}


int parser(t_data *data)
{
    if (!data || !data->token_list)
        return -1;

    t_command *current_cmd = NULL;
    t_token *current_token = data->token_list;
    int new_cmd = 1; // Flag to indicate when to start processing a new command

    while (current_token != NULL)
    {
        if (current_token->type == T_PIPE)
        {
            new_cmd = 1;
        }
        else
        {
            // Process commands or arguments
            if (new_cmd || current_token->type == T_COMMAND || current_token->type == T_ARGUMENT)
            {
                // Delegate to process_commands for command creation and argument linking
                int process_result = process_commands(data, current_token, &current_cmd);
                if (process_result != 0)
                {
                    ft_error("Error: Failed to process command or argument.\n");
                    return 1;
                }
                if (current_token->type == T_COMMAND)
                {
                    new_cmd = 0;
                }
            }
            else if (current_token->type == T_IN || current_token->type == T_OUT || current_token->type == T_APPEND || current_token->type == T_HEREDOC)
            {

                if (apply_redirection(data, current_token) != 0)
                {
                    return (1);
                }
            }
        }
        current_token = current_token->next;
    }
    return 0;
}
