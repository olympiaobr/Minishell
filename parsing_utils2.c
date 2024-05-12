/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olobresh <olobresh@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 21:30:00 by olobresh          #+#    #+#             */
/*   Updated: 2024/05/11 21:30:02 by olobresh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Libft/libft.h"
#include "includes/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(const unsigned char *)s1 - *(const unsigned char *)s2);
}

char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strcat(char *dest, const char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (dest[i] != '\0')
	{
		i++;
	}
	while (src[j] != '\0')
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dest);
}

size_t	ft_strspn(const char *str, const char *delim)
{
	const char	*p;
	const char	*s;

	s = str;
	while (*s != '\0')
	{
		p = delim;
		while (*p != '\0' && *p != *s)
			p++;
		if (*p == '\0')
			break ;
		s++;
	}
	return (s - str);
}

size_t	ft_strcspn(const char *str, const char *reject)
{
	const char	*s;
	const char	*r;

	s = str;
	while (*s != '\0')
	{
		r = reject;
		while (*r != '\0')
		{
			if (*s == *r)
				return (s - str);
			r++;
		}
		s++;
	}
	return (s - str);
}
