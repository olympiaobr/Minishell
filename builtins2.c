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

#include "Libft/libft.h"
#include "includes/minishell.h"

char *get_env_var(char **envp, const char *name)
{
    size_t len;
    int i;

    len = ft_strlen(name);
    i = 0;
    while (envp[i] != NULL)
    {
        if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
        {
            return envp[i] + len + 1;
        }
        i++;
    }
    return NULL;
}

char **expand_env(char **env, int newsize)
{
    char **new_env;
    int j;

    new_env = (char **)malloc(sizeof(char *) * newsize);
    if (new_env == NULL)
    {
        return NULL;
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
    return new_env;
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

int set_env_var(t_data *data, const char *name, const char *value)
{
    char *new_val;
    size_t name_len, value_len;
    int i = 0;

    if (!name || !value || name[0] == '\0' || name[0] == '=' || strchr(name, '=') != NULL)
    {
        fprintf(stderr, "Invalid environment variable name\n");
        return -1;
    }
    name_len = ft_strlen(name);
    value_len = ft_strlen(value);
    // Allocate memory for the new environment variable
    new_val = malloc(name_len + value_len + 2); // +2 for '=' and '\0'
    if (new_val == NULL)
    {
        perror("Memory allocation failed for environment variable");
        return -1;
    }
    ft_strncpy(new_val, name, name_len);
    new_val[name_len] = '=';
    ft_strncpy(new_val + name_len + 1, value, value_len);
    new_val[name_len + value_len + 1] = '\0';
    // search for existing variable and update it if found
    while (data->env[i] != NULL)
    {
        if (ft_strncmp(data->env[i], name, name_len) == 0 && data->env[i][name_len] == '=')
        {
            free(data->env[i]);
            data->env[i] = new_val;
            return 0;
        }
        i++;
    }
    // if var was not found, add to the environment
    char **new_env = expand_env(data->env, i + 2);
    if (new_env == NULL)
    {
        free(new_val);
        fprintf(stderr, "Failed to expand environment space\n");
        return -1;
    }
    new_env[i] = new_val;
    new_env[i + 1] = NULL;
    free(data->env);
    data->env = new_env;
    return 0;
}
