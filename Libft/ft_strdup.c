/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 14:09:57 by jasnguye          #+#    #+#             */
/*   Updated: 2023/11/27 18:28:28 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	size_t	i;
	size_t	len;
	char	*memory;

	i = 0;
	len = ft_strlen(s1) + 1;
	memory = (char *)malloc((len) * sizeof(char));
	if (memory == NULL)
	{
		return (NULL);
	}
	while (s1[i] != '\0')
	{
		memory[i] = s1[i];
		i++;
	}
	memory[i] = '\0';
	return ((char *)memory);
}

/* int main(void)
{
	const char *string = "Hello";
	char *result = ft_strdup(string);
	printf("%p\n", result);
	printf("%s\n", result);
	free (result);
} */