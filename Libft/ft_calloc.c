/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 12:44:06 by jasnguye          #+#    #+#             */
/*   Updated: 2024/05/13 12:26:05 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*memory;
	size_t	i;

	memory = (char *)malloc(count * size);
	if (memory == NULL)
	{
		return (NULL);
	}
	i = 0;
	while (i < count * size)
	{
		memory[i] = 0;
		i++;
	}
	return (memory);
}

/* int main(void)
{
	size_t elements = 5;
	size_t size = 1;
	char *result = ft_calloc(elements, size);
	printf("%p\n", result);
	printf("%s\n", result);  
	
	int i = 0;
	while (i < elements * size) 
	{
        if (result[i] != 0) {
            printf("Memory initialization failed at byte %d.\n", i);
            free(result);
            return 1;
        }
		i++;
    }
    printf("Memory successfully allocated and initialized to zero.\n");


	free(result);
} */