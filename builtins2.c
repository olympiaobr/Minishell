/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 13:12:55 by olobresh          #+#    #+#             */
/*   Updated: 2024/04/16 13:12:59 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

int is_builtin(const char *command)
{
    const char *builtins[] = {"cd", "echo", "pwd", "export", "unset", "env", "exit", NULL};
    int i;

    i = 0;
    while (builtins[i] != NULL)
    {
        if (!ft_strcmp(command, builtins[i]))
        {
            return 1;
        }
        i++;
    }
    return 0;
}


// Function to fetch an environment variable's value
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
int validate_num(const char *str)
{
    while (*str)
    {
        if (!ft_isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}

int exit_cmd(t_data *data, t_command *cmd)
{
    (void)data;
    int exit_status = 0;

    if (cmd->argc > 2)
    {
        fprintf(stderr, "exit: too many arguments\n");
        return 1;
    }
    if (cmd->argc == 2)
    {
        char *arg = cmd->argv->value;
        if (validate_num(arg))
        {
            exit_status = ft_atoi(arg);
        } else
        {
            fprintf(stderr, "exit: numeric argument required\n");
            exit_status = 255;
        }
    }
    exit(exit_status);
    return (0);
}
void remove_var(t_data *data, const char *var_name)
{
    int len = ft_strlen(var_name);
    char **current = data->env;

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
int valid_identifier(const char *name)
{
    const char *valid_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";

    if (!name || *name == '\0' || (!ft_isalpha(*name) && *name != '_'))
        return 0;
    name++;
    if (*name && ft_strspn(name, valid_chars) != ft_strlen(name))
        return 0;
    return 1;
}

int unset_cmd(t_data *data, t_command *cmd)
{
    t_token *arg = cmd->argv;

    while (arg)
    {
        if (!valid_identifier(arg->value))
        {
            fprintf(stderr, "unset: '%s' is not a valid identifier\n", arg->value);
        }
        else
        {
            ft_printf("Unsetting variable: %s\n", arg->value);
            remove_var(data, arg->value);
        }
        arg = arg->next;
    }
    return 0;
}

int export_cmd(t_data *data, t_command *cmd)
{
    t_token *arg = cmd->argv;
    char *var_name;
    char *value;

    while (arg)
    {
        var_name = custom_strtok(arg->value, "=");
        value = custom_strtok(NULL, "");
        if (!valid_identifier(var_name))
        {
            fprintf(stderr, "export: '%s': not a valid identifier\n", var_name);
        }
        else
        {
            char *existing_value = get_env_var(data->env, var_name);
            if (existing_value)
            {
                if (value)
                {
                    set_env_var(data, var_name, value);
                }
                else
                {
                    set_env_var(data, var_name, "");
                }
            }
            else
            {
                if (value)
                {
                    if (set_env_var(data, var_name, value) == -1)
                    {
                        fprintf(stderr, "Failed to export variable: %s\n", var_name);
                    }
                }
                else
                {
                    if (set_env_var(data, var_name, "") == -1)
                    {
                        fprintf(stderr, "Failed to export variable: %s\n", var_name);
                    }
                }
            }
        }
        arg = arg->next;
    }
    return 0;
}


