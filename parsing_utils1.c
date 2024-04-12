/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 18:32:49 by olobresh          #+#    #+#             */
/*   Updated: 2024/04/08 11:17:02 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

//  Sets/updates the command field in a t_command structure using the value from a t_token
int set_command(t_command *cmd_struct, t_token *current_token)
{
    if (!cmd_struct || !current_token)
		return (-1);

    if (cmd_struct->command)
    {
        free(cmd_struct->command);
        cmd_struct->command = NULL;
    }
    cmd_struct->command = ft_strdup(current_token->value);
    if (!(cmd_struct->command))
		return (-1);
    return (0);
}
int add_token_to_list(t_token **token_list, t_token *token)
{
    if (!token_list || !token)
		return (-1);

    t_token *new_node = malloc(sizeof(t_token));
    if (!new_node)
		return (-1);
    new_node->type = token->type;
    new_node->value = ft_strdup(token->value);
    new_node->is_quoted = token->is_quoted;
    new_node->next = NULL;

    if (!*token_list)
	{
        *token_list = new_node;
    }
	else
	{
        t_token *current = *token_list;
        while (current->next) current = current->next;
        current->next = new_node;
    }

    return (0);
}
// for the options/ -
int add_option_to_command(t_command *cmd, t_token *token)
{
    if (!cmd || !token)
		return (-1);
    return (add_token_to_list(&(cmd->option), token));
}
//Creates new command node from a T_COMMAND type token and appends it to the commands list in t_data.
t_command	*create_command(t_data *data, t_token *token)
{
	t_command	*new_command;
	t_command	*last_command;

	if (!token || token->type != T_COMMAND)
		return (NULL);

	new_command = (t_command *)malloc(sizeof(t_command));
	if (!new_command)
	{
		perror("Failed to allocate memory for new command node");
		return (NULL);
	}
	ft_memset(new_command, 0, sizeof(t_command));
	new_command->command = ft_strdup(token->value);
	new_command->argv = NULL;
	new_command->argc = 0;
	new_command->next = NULL;

	if (!data->commands)
	{
		data->commands = new_command;
	}
	else
	{
		last_command = data->commands;
		while (last_command->next)
			last_command = last_command->next;
		last_command->next = new_command;
	}
	return (new_command);
}
//Appends an arg token to the last command's argument list (argv)
int link_arg_to_command(t_command *last_command, t_token *token)
{
    t_token *new_arg;
    t_token *last_arg;

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
    if (!last_command->argv)
	{
        last_command->argv = new_arg;
    }
	else
	{
        last_arg = last_command->argv;
        while (last_arg->next != NULL)
		{
            last_arg = last_arg->next;
        }
        last_arg->next = new_arg;
    }
    last_command->argc += 1;
    return 0;
}

int process_commands(t_data *data, t_token *token, t_command **current_cmd)
{
	if (!data || !token || !current_cmd)
		return (-1);

	if (data->create_new_command || token->type == T_COMMAND)
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
	}
	else if (token->type == T_ARGUMENT)
	{
		if (token->value[0] == '-')
		{
			return (add_option_to_command(*current_cmd, token));
		}
		else
		{
			return (link_arg_to_command(*current_cmd, token));
		}
	}
	return (0);
}

// sets up redir for input/output based on the token's type and opens the associated file descriptor.
int setup_redirection(t_data *data, t_token *token, int oflag)
{
    char *file_path = NULL;
    int fd = -1;

    if (token->type == T_IN || token->type == T_HEREDOC)
    {
        if (data->input_file)
        {
            free(data->input_file);
        }
        data->input_file = strdup(token->value);
        file_path = data->input_file;
    }
    else
    {
        if (data->output_file)
        {
            free(data->output_file);
        }
        data->output_file = strdup(token->value);
        file_path = data->output_file;
    }
    if (!file_path)
    {
        ft_error("Error: strdup failed for file path\n");
        return 1;
    }
    fd = open(file_path, oflag, 0644);
    if (fd < 0)
    {
        ft_error("Error: Failed to open file\n");
        return 1;
    }
    if (token->type == T_IN || token->type == T_HEREDOC)
    {
        data->std_input_fd = fd;
    }
    else
    {
        data->std_output_fd = fd;
    }
    return (0);
}
// Determines the correct file open flags based on the token's type and calls setup_redirection to apply these settings.
int apply_redirection(t_data *data, t_token *token)
{
    int oflag;

    if (token->type == T_IN)
	{
        oflag = O_RDONLY;
        return (setup_redirection(data, token, oflag));
    }
    if (token->type == T_OUT)
	{
        oflag = O_CREAT | O_WRONLY | O_TRUNC;
        return (setup_redirection(data, token, oflag));
    }

    if (token->type == T_APPEND)
	{
        oflag = O_CREAT | O_WRONLY | O_APPEND;
        return (setup_redirection(data, token, oflag));
    }

    if (token->type == T_HEREDOC)
	{
        return (0);
    }
    return (1);
}

void ft_error(char *err)
{
    ft_putstr_fd(err, 2);
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
                    ft_error("Error: Failed to apply redirection.\n");
                    return 1;
                }
            }
        }
        current_token = current_token->next;
    }
    return 0;
}

void free_command(t_command *cmd)
{
    if (!cmd) return;

    free(cmd->command);
    t_token *arg = cmd->argv;
    while (arg)
    {
        t_token *next_arg = arg->next;
        free(arg->value);
        free(arg);
        arg = next_arg;
    }
    t_token *opt = cmd->option;
    while (opt)
    {
        t_token *next_opt = opt->next;
        free(opt->value);
        free(opt);
        opt = next_opt;
    }
    int index = 0;
    if (cmd->argv_array)
    {
        while (index < cmd->argc)
         {
            free(cmd->argv_array[index]);
            index++;
        }
        free(cmd->argv_array);
    }
    free(cmd);
}

void free_commands(t_command *cmd)
{
    while (cmd)
    {
        t_command *next_cmd = cmd->next;
        free_command(cmd);
        cmd = next_cmd;
    }
}
