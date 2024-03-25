/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 11:57:26 by jasnguye          #+#    #+#             */
/*   Updated: 2023/11/28 14:13:42 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	int			i;
	char		*dest_pointer;
	const char	*src_pointer; 

	i = 0;
	dest_pointer = (char *)dest;
	src_pointer = (const char *)src;
	if (dest == NULL && src == NULL)
	{
		return (dest);
	}
	while (n > 0)
	{
		dest_pointer[i] = src_pointer[i];
		i++;
		n--;
	}
	return (dest);
}

/* int main(void)
{
	char destinationString[100];
	char *sourceString = "Hello";
	size_t n = 4 * sizeof(char);

	ft_memcpy(destinationString, sourceString, n);
	destinationString[n] = '\0';
	printf("%s\n", destinationString);
} */

/* int main(void)
{
	int dest[4];
	int source[4] = {1, 2, 3, 4};
	size_t n = sizeof(int) *4;
	ft_memcpy(dest, source, n);
	int i = 0;
	while (i < 4)
	{
		printf("%d", dest[i]);
		i++;
	}
	printf("\n");
} */