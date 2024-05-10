/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 15:48:00 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/10 15:48:02 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

void ft_error(char *err)
{
    ft_putstr_fd(err, 2);
}
void append_slash(char **directory)
{
    char *temp;

    temp = *directory;
    *directory = ft_strjoin(temp, "/");
    if (!(*directory))
	{
        perror("Error appending slash to PATH directory");
        free(temp);
        exit(EXIT_FAILURE);
    }
    free(temp);
}

int match_found(const char *haystack, const char *needle)
{
    while (*haystack && *needle && *haystack == *needle)
    {
        haystack++;
        needle++;
    }
    return (*needle == '\0');
}

char *cust_strstr(const char *haystack, const char *needle)
{
    if (*needle == '\0')
    {
        return ((char *)haystack);
    }

    while (*haystack != '\0')
    {
        if (*haystack == *needle)
        {
            if (match_found(haystack, needle))
            {
                return ((char *)haystack);
            }
        }
        haystack++;
    }
    return (NULL);
}

char *cust_getenv(const char *name, t_data *data)
{
    int i = 0;
    char *env_entry;
    char *found;

    while ((env_entry = data->env[i]) != NULL)
    {
        found = cust_strstr(env_entry, name);
        if (found && found[ft_strlen(name)] == '=' && (found == env_entry || *(found - 1) == '\0'))
        {
            return (found + ft_strlen(name) + 1);
        }
        i++;
    }
    return (NULL);
}


