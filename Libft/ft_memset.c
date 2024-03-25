/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 17:34:07 by jasnguye          #+#    #+#             */
/*   Updated: 2023/11/27 18:13:26 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *str, int c, size_t n)
{
	char	*pointer;

	pointer = (char *) str;
	while (n > 0)
	{
		*pointer = c;
		pointer++;
		n--;
	}
	return (str);
}

/* int main(void)
{
	char myString[] = "Hellohello";
	int c = 'a';
	size_t n = 5;

	printf("Before memset: %s\n", myString);
	memset(myString +1, c, n);
	printf("After memset: %s\n", myString);
	
} */