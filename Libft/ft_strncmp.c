/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 15:32:05 by jasnguye          #+#    #+#             */
/*   Updated: 2023/11/27 18:40:44 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] != '\0' || s2[i] != '\0'))
	{
		if ((unsigned char)s1[i] != (unsigned char) s2[i])
		{
			return ((int)(unsigned char) s1[i] - (unsigned char)s2[i]);
		}
		i++;
	}
	return (0);
}

/* int main(void)
{
	char *string1 = "Hello";
	char *string2 = "HellO";
	size_t n = 5;
	printf("%d\n", ft_strncmp(string1, string2, n));
} */