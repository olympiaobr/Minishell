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


int cd_cmd(t_data *data, t_command *cmd)
{
    char *target;
    char *arg_value;
    char *newpwd;
    char *oldpwd;
    int result;

    if (cmd->argc == 1)
    {
        target = get_env_var(data->env, "HOME");
        if (!target)
        {
            target = "/";
        }
    }
    else if (cmd->argc == 2)
    {
        arg_value = cmd->argv->value;
        if (ft_strcmp(arg_value, "-") == 0)
        {
            target = get_env_var(data->env, "OLDPWD");
            if (!target)
            {
                fprintf(stderr, "cd: OLDPWD not set\n");
                return EXIT_FAILURE;
            }
            printf("%s\n", target);
        }
        else if (ft_strcmp(arg_value, "~") == 0)
        {
            target = get_env_var(data->env, "HOME");
            if (!target)
            {
                target = "/";
            }
        }
        else
        {
            target = arg_value;
        }
    }
    else
    {
        fprintf(stderr, "cd: Incorrect usage. Expected one argument or none, but got %d.\n", cmd->argc - 1);
        return EXIT_FAILURE;
    }
    if (chdir(target) != 0)
    {
        perror("cd failed");
        return EXIT_FAILURE;
    }
    newpwd = getcwd(NULL, 0);
    if (!newpwd)
    {
        perror("getcwd failed");
        return EXIT_FAILURE;
    }
    oldpwd = get_env_var(data->env, "PWD");
    result = set_env_var(data, "OLDPWD", oldpwd);
    if (result != 0)
    {
        fprintf(stderr, "cd: error updating OLDPWD environment variable\n");
        free(newpwd);
        return EXIT_FAILURE;
    }
    result = set_env_var(data, "PWD", newpwd);
    if (result != 0)
    {
        fprintf(stderr, "cd: error updating PWD environment variable\n");
        free(newpwd);
        return EXIT_FAILURE;
    }
    free(newpwd);
    return EXIT_SUCCESS;
}
