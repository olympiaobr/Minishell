/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasnguye <jasnguye@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 12:33:32 by jasnguye          #+#    #+#             */
/*   Updated: 2023/11/27 18:15:52 by jasnguye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	digit; 

	if (n == -2147483648)
	{
		write (fd, "-2147483648", 11);
	}
	else 
	{
		if (n < 0)
		{
			write(fd, "-", 1);
			n = -n;
		}
		if (n > 9)
		{
			ft_putnbr_fd (n / 10, fd);
			n %= 10;
		}
		digit = n + '0';
		write(fd, &digit, 1);
	}
}

/* int main(void)
{
	int file_descriptor = 1;
	int n = 10234;
	ft_putnbr_fd(n, file_descriptor);
} */