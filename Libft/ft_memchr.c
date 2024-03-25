/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 15:59:47 by jasnguye          #+#    #+#             */
/*   Updated: 2023/11/27 19:02:00 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*str;
	unsigned char	cc;

	i = 0;
	str = (unsigned char *)s;
	cc = (unsigned char)c;
	while (i < n)
	{
		if (str[i] == cc)
		{
			return ((void *)&str[i]);
		}
		i++;
	}
	return (NULL);
}

/* int main(void)
{
	const char *string = "Hello my name is Jasmin";
	int find = 121;
	size_t n = 25;
	char *result = ft_memchr(string, find, n);
	printf("%p\n", result);
	printf("%c\n", *result);
} */

/* int main(void)
{
    int array[3] = {1, 2, 3};
    int find = 3;
    size_t n = sizeof(array); // Correctly getting the size in bytes
    void* result = ft_memchr(array, find, n);

    printf("Result: %p\n", result);
	printf("Result: %d\n", *(int*) result);

    return 0;
} */
