/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 13:42:23 by jasnguye          #+#    #+#             */
/*   Updated: 2023/12/07 13:44:58 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	f(unsigned int i, char c)
{
	char	conversion;

	conversion = c;
	if (i % 2 == 0 && c >= 'a' && c <= 'z')
	{
		conversion = c - 32;
	}
	return (conversion);
}

char	*ft_strmapi(const char *s, char (*f)(unsigned int, char))
{
	int		i;	
	size_t	len;
	char	*newstring;

	i = 0;
	len = ft_strlen(s);
	newstring = (char *)malloc((len + 1) * sizeof(char));
	if (newstring == NULL || s == NULL)
	{
		return (NULL);
	}
	while (s[i] != '\0')
	{
		newstring[i] = f(i, s[i]);
		i++;
	}
	newstring[i] = '\0';
	return (newstring);
}

/* int main(void)
{
	char string[] = "abc";
	char *result = ft_strmapi(string, f);
	printf("%s\n", result);
	free(result);
} */