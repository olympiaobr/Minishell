/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:11:24 by jasnguye          #+#    #+#             */
/*   Updated: 2023/11/28 11:02:21 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char		*dest_pointer;
	const char	*src_pointer;
	size_t		i;

	dest_pointer = (char *) dest;
	src_pointer = (const char *) src;
	if (dest == NULL && src == NULL)
		return (dest);
	if (dest_pointer > src_pointer)
	{
		while (n--)
		{
			dest_pointer[n] = src_pointer[n];
		}
	}
	else 
	{
		i = 0;
		while (i < n)
		{
			dest_pointer[i] = src_pointer[i];
			i++;
		}
	}
	return (dest);
}

/* int main(void)
{
	
	int source[4] = {1, 2, 3, 4};
	size_t n = sizeof(int) *3;
	ft_memmove(source + 1, source, n);
	int i = 0;
	while (i < 4)
	{
		printf("%d", source[i]);
		i++;
	}
	printf("\n");
} */
/* 
int main(void)
{
	char sourceString[] = "Hello";
	size_t n = 4 * sizeof(char);

	ft_memmove(sourceString + 1, sourceString, n);
	int i = 0;
	while (i < 5)
	{
		printf("%c", sourceString[i]);
		i++;
	}
	printf("\n");
} */
