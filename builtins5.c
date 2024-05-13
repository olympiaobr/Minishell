/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 13:50:04 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/13 12:13:08 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

void	remove_var(t_data *data, const char *var_name)
{
	int		len;
	char	**current;

	len = ft_strlen(var_name);
	current = data->env;
	while (*current && ft_strncmp(*current, var_name, len))
	{
		current++;
	}
	if (*current && (*current)[len] == '=')
	{
		free(*current);
		while (*(current + 1))
		{
			*current = *(current + 1);
			current++;
		}
		*current = NULL;
	}
	else
	{
		ft_printf("Variable not found.\n");
	}
}

int	valid_identifier(const char *name)
{
	const char	*valid_chars;

	valid_chars =
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789_";
	if (!name || *name == '\0' || (!ft_isalpha(*name) && *name != '_'))
		return (0);
	name++;
	if (*name && ft_strspn(name, valid_chars) != ft_strlen(name))
		return (0);
	return (1);
}

int	unset_cmd(t_data *data, t_command *cmd)
{
	t_token	*arg;

	arg = cmd->argv;
	while (arg)
	{
		if (!valid_identifier(arg->value))
		{
			ft_printf("unset: %s is not a valid identifier\n", arg->value);
		}
		else
		{
			remove_var(data, arg->value);
		}
		arg = arg->next;
	}
	return (0);
}

void	process_var_update(t_data *data, char *var_name, char *value)
{
	char	*existing_value;

	existing_value = get_env_var(data->env, var_name);
	if (existing_value)
	{
		if (value)
			set_env_var(data, var_name, value);
		else
			set_env_var(data, var_name, existing_value);
	}
	else
	{
		if (value)
		{
			if (set_env_var(data, var_name, value) == -1)
				printf("Failed to export variable: %s\n", var_name);
		}
		else
		{
			if (set_env_var(data, var_name, "") == -1)
				printf("Failed to export variable: %s\n", var_name);
		}
	}
}

void	handle_export_operation(t_data *data, t_token *arg)
{
	char	*var_name;
	char	*value;

	while (arg)
	{
		var_name = custom_strtok(arg->value, "=");
		value = custom_strtok(NULL, "");
		if (!valid_identifier(var_name))
		{
			printf("export: '%s': not a valid identifier\n", var_name);
		}
		else
		{
			process_var_update(data, var_name, value);
		}
		arg = arg->next;
	}
}
