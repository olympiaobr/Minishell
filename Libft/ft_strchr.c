/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 14:09:02 by jasnguye          #+#    #+#             */
/*   Updated: 2023/11/27 18:26:15 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
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

/* int main(void)
{
	const char *string = "Hello my name is Jasmin";
	int find = 121;
	char *result = ft_strchr(string, find);
	printf("%p\n", result);
	printf("%c\n", *result);
} */