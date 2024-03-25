/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 14:55:05 by jasnguye          #+#    #+#             */
/*   Updated: 2023/11/27 18:43:58 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;
	char	cc;

	i = ft_strlen(s);
	cc = (char) c;
	if (cc == '\0')
	{
		return ((char *)&s[i]);
	}
	while (i > 0)
	{
		if (s[i - 1] == cc)
		{
			return ((char *)&s[i - 1]);
		}
		i--;
	}
	return (NULL);
}

/* int main(void)
{
	const char *string = "Hello my name is Jasmin";
	int find = 105;
	char *result = ft_strrchr(string, find);
	printf("%p\n", result);
	printf("%c\n", *result);
} */