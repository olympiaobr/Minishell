/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 18:33:04 by jasnguye          #+#    #+#             */
/*   Updated: 2023/11/27 18:31:41 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	size_t	len;
	char	*memory;

	i = 0;
	j = 0;
	len = ft_strlen(s1) + ft_strlen(s2) + 1;
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
	while (s2[j] != '\0')
	{
		memory[i + j] = s2[j];
		j++;
	}
	memory[i + j] = '\0';
	return (memory);
}

/* int main(void)
{
	char *string1 = "Hello";
	char *string2 = "hello";
	char *result = ft_strjoin(string1, string2);        
	printf("%p\n", result);
	printf("%s\n", result);    
	free(result);    
} */