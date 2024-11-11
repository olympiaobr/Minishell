/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:12:55 by olobresh          #+#    #+#             */
/*   Updated: 2024/04/25 17:05:54 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Libft/libft.h"
#include "../includes/minishell.h"

int	echo_cmd(t_command *cmd)
{
	t_token	*current_arg;
	int		newline;
	int		first;

	current_arg = cmd->option;
	newline = 1;
	first = 1;
	newline = print_arg(current_arg, &first, newline);
	current_arg = cmd->argv;
	newline = print_arg(current_arg, &first, newline);
	if (newline)
		ft_printf("\n");
	return (0);
}

char	*get_env_var(char **envp, const char *name)
{
	size_t	len;
	int		i;

	len = ft_strlen(name);
	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
		{
			return (envp[i] + len + 1);
		}
		i++;
	}
	return (NULL);
}

char	**expand_env(char **env, int newsize)
{
	char	**new_env;
	int		j;

	new_env = (char **)malloc(sizeof(char *) * newsize);
	if (new_env == NULL)
	{
		return (NULL);
	}
	j = 0;
	while (env[j] != NULL)
	{
		new_env[j] = env[j];
		j++;
	}
	while (j < newsize)
	{
		new_env[j] = NULL;
		j++;
	}
	return (new_env);
}

char	*ft_strncpy(char *dest, const char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (src[i] != '\0' && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

int	add_new_env(t_data *data, char *new_val, int index)
{
	char	**new_env;

	new_env = expand_env(data->env, index + 2);
	if (new_env == NULL)
	{
		free(new_val);
		printf("Failed to expand environment space\n");
		return (-1);
	}
	new_env[index] = new_val;
	new_env[index + 1] = NULL;
	free(data->env);
	data->env = new_env;
	return (0);
}
