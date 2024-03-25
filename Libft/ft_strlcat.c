/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 11:43:44 by jasnguye          #+#    #+#             */
/*   Updated: 2023/11/27 19:03:16 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	src_len;
	size_t	dest_len;
	size_t	i;

	src_len = ft_strlen(src);
	dest_len = ft_strlen(dest);
	i = 0;
	if (size > dest_len && size > 0)
	{
		while (src[i] != '\0' && dest_len + i < size - 1)
		{
			dest[dest_len + i] = src[i];
			i++;
		}
		dest[dest_len + i] = '\0';
	}
	if (size > dest_len)
	{
		return (dest_len + src_len);
	}
	else
	{
		return (size + src_len);
	}
}

/* int main(void)
{
	char destination[100] = "Hello";
	char *source = "Hello";
	size_t n = 6;
	printf("%ld\n", ft_strlcat(destination, source, n));
} */
