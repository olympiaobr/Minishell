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

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	front;
	size_t	rear;
	char	*newstring;

	front = 0;
	rear = ft_strlen(s1);
	newstring = 0;
	if (s1 != 0 && set != 0)
	{
		while (s1[front] != '\0' && ft_strchr(set, s1[front]))
			front++;
		while (s1[rear - 1] != '\0' && ft_strchr(set, s1[rear - 1])
			&& rear > front)
		{
			rear--;
		}
		if (front > rear)
		{
			return (ft_strdup(""));
		}
		newstring = (char *)malloc(sizeof(char) * (rear - front + 1));
		 if (newstring == NULL)
        {
            return NULL;
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
