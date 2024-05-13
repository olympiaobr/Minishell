/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 17:37:05 by jasnguye          #+#    #+#             */
/*   Updated: 2024/05/13 12:25:59 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2; 

	i = 0;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (i < n)
	{
		if (str1[i] != str2[i])
		{
			return (str1[i] - str2[i]);
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
	int result = ft_memcmp(string1, string2, n);
	printf("%d\n", result);
} */
/* int main(void)
{
    int array1[3] = {1, 2, 3};
    int array2[3] = {1, 2, 3};
    size_t n = sizeof(array1);
    int result = ft_memcmp(array1, array2, n);

    printf("Result: %d\n", result);

    return 0;
} */