/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:54:54 by jasnguye          #+#    #+#             */
/*   Updated: 2024/01/05 11:22:25 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	s_len;
	char	*substring;

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

/* int main(void)
{
	const char *string = "abcdefghijklmnop";
	unsigned int substart = 3;
	size_t len = 4;
	char *result = ft_substr(string, substart, len);
	printf("%p\n", result);
	printf("%s\n", result);
	free (result);
} */