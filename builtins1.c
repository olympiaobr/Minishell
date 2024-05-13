/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 17:32:07 by olobresh          #+#    #+#             */
/*   Updated: 2024/04/25 13:05:29 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

int	pwd_cmd(void)
{
	char	*cwd;
	int		write_result;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("minishell: pwd");
		return (EXIT_FAILURE);
	}
	write_result = ft_putendl_fd(cwd, STDOUT_FILENO);
	free(cwd);
	if (write_result < 0)
	{
		perror("minishell: write error");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	print_environment(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strchr(envp[i], '='))
		{
			if (ft_putendl_fd(envp[i], STDOUT_FILENO) < 0)
			{
				perror("minishell: write error");
				return (EXIT_FAILURE);
			}
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

int	env_cmd(t_data *data)
{
	char	**envp;

	envp = data->env;
	if (!envp)
	{
		printf("Environment uninitialized.\n");
		return (EXIT_FAILURE);
	}
	if (data->commands && data->commands->argc > 1)
	{
		ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	return (print_environment(envp));
}

int	n_option(const char *arg)
{
	int	j;

	if (arg[0] != '-')
		return (0);
	j = 1;
	while (arg[j] != '\0')
	{
		if (arg[j] != 'n')
		{
			return (0);
		}
		j++;
	}
	return (1);
}

int	print_arg(t_token *current_arg, int *first, int newline)
{
	while (current_arg)
	{
		if (!n_option(current_arg->value))
		{
			if (!(*first))
				ft_printf(" ");
			ft_printf("%s", current_arg->value);
			*first = 0;
		}
		else
		{
			newline = 0;
		}
		current_arg = current_arg->next;
	}
	return (newline);
}
