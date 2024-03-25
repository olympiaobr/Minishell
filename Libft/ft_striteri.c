/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/25 15:17:25 by jasnguye          #+#    #+#             */
/*   Updated: 2023/11/29 11:07:41 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* void	f_function(unsigned int i, char *c)
{
	if (i % 2 == 0 && *c >= 'a' && *c <= 'z')
	{
		*c = *c - 32;
	}
} */

void	ft_striteri(char *s, void (*f_function)(unsigned int, char*))
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		(*f_function)(i, &s[i]);
		i++;
	}
}

/* int main(void)
{
	char string[] = "abc";
	printf("%s\n", string);
	ft_striteri(string, f_function);
	printf("%s\n", string);
} */
