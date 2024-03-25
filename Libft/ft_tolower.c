/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 13:59:21 by jasnguye          #+#    #+#             */
/*   Updated: 2023/11/27 18:56:31 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_tolower(int c)
{
	int	i;

	if (c >= 65 && c <= 90)
	{
		i = c + 32;
		return (i);
	}
	else 
	{
		return (c);
	}
}

/* int main(void)
{
	printf("%d\n", ft_tolower(122));
} */