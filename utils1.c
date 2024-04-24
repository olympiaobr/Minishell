/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 15:22:21 by olobresh          #+#    #+#             */
/*   Updated: 2024/04/10 15:22:32 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

void free_command(t_command *cmd)
{
    if (!cmd)
		return;

	free(cmd->path);
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

void free_all(t_data *data)
{
    int i = 0;

    // free environment var
    while (data->env && data->env[i])
    {
        free(data->env[i]);
        data->env[i] = NULL;
        i++;
    }
    free(data->env);
    data->env = NULL;

    // free path directories
    if (data->path_dirs)
    {
        i = 0;
        while (data->path_dirs[i])
        {
            free(data->path_dirs[i]);
            data->path_dirs[i] = NULL;
            i++;
        }
        free(data->path_dirs);
        data->path_dirs = NULL;
    }
    free_tokens(data);
    free_commands(data->commands);
    free(data->input_file);
    data->input_file = NULL;
    free(data->output_file);
    data->output_file = NULL;
}

