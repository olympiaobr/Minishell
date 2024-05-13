/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 15:49:05 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/13 12:25:31 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

void	setup_io_channels(int *io)
{
	if (io[0] != STDIN_FILENO)
	{
		dup2(io[0], STDIN_FILENO);
		close(io[0]);
	}
	if (io[1] != STDOUT_FILENO)
	{
		dup2(io[1], STDOUT_FILENO);
		close(io[1]);
	}
}

char	**create_argv(t_command *cmd)
{
	int		argc;
	char	**argv;

	argc = count_arguments(cmd);
	argv = malloc(sizeof(char *) * (argc + 1));
	if (!argv)
	{
		perror("Memory allocation failed for argv");
		return (NULL);
	}
	return (populate_argv(cmd, argv));
}

int	count_arguments(t_command *cmd)
{
	int		argc;
	t_token	*arg;

	argc = 1;
	if (cmd->option != NULL)
	{
		argc++;
	}
	arg = cmd->argv;
	while (arg)
	{
		argc++;
		arg = arg->next;
	}
	return (argc);
}

int	validate_io_channels(int *io)
{
	if (io[0] < 0 || io[1] < 0)
	{
		fprintf(stderr, "Error: Invalid IO channels determined.\n");
		return (0);
	}
	return (1);
}

int	validate_command(t_command *cmd)
{
	if (!cmd->path || access(cmd->path, X_OK) != 0)
	{
		perror("Invalid command path or command is not executable");
		return (0);
	}
	return (1);
}
