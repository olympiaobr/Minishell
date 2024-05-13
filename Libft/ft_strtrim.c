/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 10:28:12 by jasnguye          #+#    #+#             */
/*   Updated: 2023/11/27 18:53:43 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_find_trim_indices(char const *s1, char const *set, size_t *front,
		size_t *rear)
{
	*front = 0;
	*rear = ft_strlen(s1);
	while (s1[*front] != '\0' && ft_strchr(set, s1[*front]))
	{
		(*front)++;
	}
	while (*rear > *front && s1[*rear - 1] != '\0' && ft_strchr(set, s1[*rear
				- 1]))
	{
		(*rear)--;
	}
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	front;
	size_t	rear;
	char	*newstring;

	newstring = NULL;
	if (s1 != NULL && set != NULL)
	{
		ft_find_trim_indices(s1, set, &front, &rear);
		if (front > rear)
		{
			return (ft_strdup(""));
		}
		newstring = (char *)malloc(sizeof(char) * (rear - front + 1));
		if (newstring == NULL)
		{
			return (NULL);
		}
		ft_strlcpy(newstring, &s1[front], rear - front + 1);
	}
	return (newstring);
}

/* int main(void)
{
	const char *string = "xyxyMy name is Jasmin.xyyx";
	const char *trim = "xy";
	char *result = ft_strtrim(string, trim);
	printf("%p\n", result);
	printf("%s\n", result);
	free (result);
} */
