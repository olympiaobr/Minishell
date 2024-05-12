/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 12:11:55 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/12 12:11:57 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

t_command *create_command(t_data *data, t_token *token)
{
    t_command *new_command;

    new_command = (t_command *)malloc(sizeof(t_command));
    if (!new_command)
    {
        perror("Failed to allocate memory for new command node");
        return NULL;
    }

    ft_memset(new_command, 0, sizeof(t_command));
    new_command->command = ft_strdup(token->value);
    new_command->argv = NULL;
    new_command->argc = 1;
    new_command->next = NULL;
    link_command(data, new_command);
    return new_command;
}
static void append_arg_to_command(t_command *last_command, t_token *new_arg)
{
    t_token *current;

    if (!last_command->argv)
    {
        last_command->argv = new_arg;
    }
    else
    {
        current = last_command->argv;
        while (current->next)
            current = current->next;
        current->next = new_arg;
    }
    last_command->argc++;
}
int link_arg_to_command(t_command *last_command, t_token *token)
{
    t_token *new_arg;

    if (!last_command || !token || token->type != T_ARGUMENT)
    {
        return -1;
    }
    new_arg = (t_token *)malloc(sizeof(t_token));
    if (!new_arg)
    {
        perror("Failed to allocate memory for new argument");
        return -1;
    }
    new_arg->type = token->type;
    new_arg->value = ft_strdup(token->value);
    if (!new_arg->value)
    {
        free(new_arg);
        return -1;
    }
    new_arg->next = NULL;
    new_arg->is_quoted = token->is_quoted;
    append_arg_to_command(last_command, new_arg);
    return 0;
}

int handle_command_creation(t_data *data, t_token *token, t_command **current_cmd)
{
    if (token->type != T_PIPE)
    {
        *current_cmd = create_command(data, token);
        if (!(*current_cmd))
            return (-1);
        if (set_command(*current_cmd, token) != 0)
            return (-1);
    }
    data->create_new_command = 0;
    return 0;
}

int process_commands(t_data *data, t_token *token, t_command **current_cmd)
{
    if (!data || !token || !current_cmd)
        return (-1);

    if (data->create_new_command || token->type == T_COMMAND)
    {
        return handle_command_creation(data, token, current_cmd);
    }
    else if (token->type == T_ARGUMENT)
    {
        if (token->value[0] == '-')
        {
            return add_option_to_command(*current_cmd, token);
        }
        else
        {
            return link_arg_to_command(*current_cmd, token);
        }
    }
    return 0;
}

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

// sets up redir for input/output based on the token's type and opens the associated file descriptor.
int setup_redirection(t_data *data, t_token *token, char *filename)
{
    int flags;

    if (token->type == T_OUT)
    {
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    }
    else if (token->type == T_APPEND)
    {
        // Set flags for append
        flags = O_WRONLY | O_CREAT | O_APPEND;
    }
    else if (token->type == T_IN || token->type == T_HEREDOC) {
        // Flags for input redirection
        flags = O_RDONLY;
    }
    printf("filename in setup redirection: %s\n", filename);
    char *file_path = ft_strdup(filename);
    if (!file_path)
    {
        perror("Failed to allocate memory for file path");
        return -1;
    }
    int fd = open(file_path, flags, 0644);
    if (fd < 0)
    {
        free(file_path);
        return -1;
    }
    clear_previous_redirections(data, token);
    if (token->type == T_IN || token->type == T_HEREDOC)
    {
        data->input_file = file_path;
        data->std_input_fd = fd;
    }
    else
    {
        setup_append_mode(data, fd, token->type);
        data->output_file = file_path;
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
