/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 12:40:04 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/12 12:40:06 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

void clear_previous_redirections(t_data *data, t_token *token)
{
    if (token->type == T_OUT || (token->type == T_APPEND && !data->append))
    {
        if (data->output_file && data->std_output_fd != STDOUT_FILENO)
        {
            close(data->std_output_fd);
            free(data->output_file);
            data->output_file = NULL;
            data->std_output_fd = STDOUT_FILENO;
        }
    }
    else if (token->type == T_IN || token->type == T_HEREDOC)
    {
        if (data->input_file && data->std_input_fd != STDIN_FILENO)
        {
            close(data->std_input_fd);
            free(data->input_file);
            data->input_file = NULL;
            data->std_input_fd = STDIN_FILENO;
        }
    }
}

void setup_append_mode(t_data *data, int fd, token_type type)
{
    if (type == T_APPEND)
        data->append = 1;
    else
        data->append = 0;
    if (data->std_output_fd != STDOUT_FILENO)
    {
        close(data->std_output_fd);
    }
    if (type == T_APPEND || type == T_OUT)
    {
        data->std_output_fd = fd;
    }
    else
    {
        data->std_output_fd = STDOUT_FILENO;
    }
}

int determine_flags(token_type type) {
    if (type == T_OUT) {
        return O_WRONLY | O_CREAT | O_TRUNC;
    } else if (type == T_APPEND) {
        return O_WRONLY | O_CREAT | O_APPEND;
    } else if (type == T_IN || type == T_HEREDOC) {
        return O_RDONLY;
    }
    return 0; // Default to no flags if the type is unrecognized
}

int open_redirection_file(char *filename, int flags) {
    char *file_path = ft_strdup(filename);
    if (!file_path) {
        perror("Failed to allocate memory for file path");
        return -1;
    }
    int fd = open(file_path, flags, 0644);
    if (fd < 0) {
        free(file_path);
        perror("Failed to open file");
        return -1;
    }
    free(file_path); // Free the duplicated path after opening the file
    return fd;
}

int setup_redirection(t_data *data, t_token *token, char *filename) {
    int fd;
    int flags = determine_flags(token->type); // Determine the appropriate flags for opening the file

    if ((fd = open_redirection_file(filename, flags)) < 0) {
        return -1; // File opening failed
    }

    clear_previous_redirections(data, token); // Clear any existing redirections based on token type

    if (token->type == T_IN || token->type == T_HEREDOC) {
        data->input_file = ft_strdup(filename);
        data->std_input_fd = fd;
    } else {
        setup_append_mode(data, fd, token->type);
        data->output_file = ft_strdup(filename);
    }
    return 0;
}


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
