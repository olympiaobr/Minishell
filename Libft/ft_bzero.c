/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 10:08:09 by jasnguye          #+#    #+#             */
/*   Updated: 2023/11/27 14:56:11 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *str, size_t n)
{
	char	*pointer;

	pointer = (char *)str;
	while (n > 0)
	{
		*pointer = 0;
		pointer++;
		n--;
	}
}

/* int	main(void)
{
	size_t	i;
	char	string[] = "Hellohello";
	size_t	n = 5;
	i = 0;
	
	printf("Before bzero: %s\n", string);
	ft_bzero(string, n);
	printf("After bzero: ");
	while (i < sizeof(string))
	{
		if (string[i] == 0)
		{
			printf("\\0 ");
		}
		else 
		{
			printf("%c", string[i]);
		}
		i++;
	}
	printf("\n");
} */
