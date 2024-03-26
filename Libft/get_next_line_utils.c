/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/31 14:45:28 by jasnguye          #+#    #+#             */
/*   Updated: 2024/03/26 14:54:25 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*gft_strchr(const char *s, int c)
{
	int		i;
	char	cc;

	i = 0;
	cc = (char) c;
	while (s[i] != '\0')
	{
		if (s[i] == cc)
		{
			return ((char *)&s[i]);
		}
		i++;
	}
	if (cc == '\0')
	{
		return ((char *)&s[i]);
	}
	return (NULL);
}

char	*gft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	*memory;

	i = 0;
	j = 0;
	len = ft_strlen(s1) + ft_strlen(s2) + 1;
	memory = (char *)malloc((len) * sizeof(char) + 2);
	if (memory == NULL)
	{
		return (NULL);
	}
	while (s1[i] != '\0')
	{
		memory[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		memory[i + j] = s2[j];
		j++;
	}
	memory[i + j] = '\0';
	return (memory);
}

char	*gft_strdup(const char *s1)
{
	size_t	i;
	size_t	len;
	char	*memory;

	i = 0;
	len = ft_strlen(s1) + 1;
	memory = (char *)malloc((len) * sizeof(char));
	if (memory == NULL)
	{
		return (NULL);
	}
	while (s1[i] != '\0')
	{
		memory[i] = s1[i];
		i++;
	}
	memory[i] = '\0';
	return ((char *)memory);
}

char	*ft_substring(char const *s, unsigned int start, unsigned int len)
{
	unsigned int	i;
	unsigned int	s_len;
	char			*substring;

	i = 0;
	s_len = ft_strlen(s);
	if (start > s_len)
	{
		return (ft_strdup(""));
	}
	if (len > s_len - start) 
	{
		len = s_len - start;
	}
	substring = (char *)malloc(len * sizeof(char) + 1);
	if (substring == NULL)
	{
		return (NULL);
	}
	while (i < len)
	{
		substring[i++] = s[start++];
	}
	substring[i] = '\0';
	return (substring);
}

int	gft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		i++;
	}
	return (i);
}
