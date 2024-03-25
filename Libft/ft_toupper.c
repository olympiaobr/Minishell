/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 12:56:51 by jasnguye          #+#    #+#             */
/*   Updated: 2023/11/27 18:56:55 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	int	i;

	if (c >= 97 && c <= 122)
	{
		i = c - 32;
		return (i);
	}
	else
	{
		return (c);
	}
}

/* int main(void)
{
	printf("%d\n", ft_toupper(97));
}
 */